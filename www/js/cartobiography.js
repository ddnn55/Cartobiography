var strokeWidth = 1.0
  , waypointRadius = 2.0
  , π = 3.14159265359
  , fauxCartoPower = 0.3
  ;

var all, scale = 1.0;

function makeFauxCartoProjection(_points, size)
{
  var mercator = d3.geo.mercator().scale(1).translate([0, 0]);

  var points = _points.map(function(pt) {
    return mercator([pt.lon, pt.lat]);
  });

  function compareNumbers(a,b) { return a - b; };
  var xs = points.map(function(p){ return p[0]; }).sort(compareNumbers);
  var ys = points.map(function(p){ return p[1]; }).sort(compareNumbers);

  var fauxCarto1d = function(ticks, size) {
    var span = ticks[ticks.length-1] - ticks[0];
    console.log('span', span);
    return function(from) {
      if(from <= ticks[0]) return 0;
      if(from >= ticks[ticks.length-1]) return size;
      var fromFraction = (from - ticks[0]) / (ticks[ticks.length-1] - ticks[0]);
      var low = 0, high = ticks.length-1, test;
      while( (test = Math.floor((low+high)/2)) != low ) {
        if(ticks[test] > from)
	  high = test;
	else
	  low = test;
      }
      var binPos = (from - ticks[low]) / (ticks[high] - ticks[low]);
      var toFraction = (low + binPos) / ticks.length;
      //var to = ticks[0] + totalPos * span;
      //var to = totalPos * size;
      //console.log('totalPos, from, to', totalPos, from, to);
      return (fauxCartoPower * toFraction + (1 - fauxCartoPower) * fromFraction) * size;
    }
  }

  console.log('x ticks', xs.length, 'y ticks', ys.length);

  var x = fauxCarto1d(xs, size[0]);
  var y = fauxCarto1d(ys, size[1]);

  var fauxCartoProjection = d3.geo.projection(function(λ, φ) {
    var lon = λ * 180 / π, lat = φ * 180 / π;
    var pMercator = mercator([lon, lat]);
    return [
      x(pMercator[0]),
      -y(pMercator[1])
    ];
  })
    .scale(1)
    .translate([0, 0])
    ;

  return fauxCartoProjection;
}


var svg = d3.select("body").append("svg:svg")
    .attr("class", "Blues")
    .attr("width", window.width)
    .attr("height", window.height);
    //.call(zoom);

var geoclip = svg.append("svg:g")
    .attr("id", "geoclip");

var map = geoclip.append("svg:g")
    .attr("id", "map");

var states = map.append("svg:g")
    .attr("id", "states");

var waypoints = geoclip.append("svg:g")
    .attr("id", "waypoints");



/*d3.json("us-states.json", function(json) {
  states.selectAll("path")
      .data(json.features)
    .enter().append("svg:path")
      .attr("d", path);
});*/


$(document).ready(function() {
//d3.json("openpaths_davidstolarsky.json", function(op) {
  //console.log(op.length + " OpenPaths waypoints");
  d3.json("photos.json", function(photos) {

    //photos = photos.splice(0, 100);
    console.log(photos.length + " geotagged photos");

    var viewportSize = [$(window).width(), $(window).height()];

    var viewportBounds = d3.geom.polygon([
      [0, 0],
      [0, viewportSize[1]],
      [viewportSize[0], viewportSize[1]],
      [viewportSize[0], viewportSize[1]]
    ]);

   



    // make 1d-1d-faux-cartogram projection
    var fauxCartoProjection = makeFauxCartoProjection(photos, [$(window).width(), $(window).height()]);
    var fauxCartoPath = d3.geo.path()
          .projection(fauxCartoProjection);

    d3.json("world-110m.json", function(error, world) {
      map.insert("path", ".graticule")
          .datum(topojson.object(world, world.objects.land))
          .attr("class", "land")
          .attr("d", fauxCartoPath);
    
      map.insert("path", ".graticule")
          .datum(topojson.mesh(world, world.objects.countries, function(a, b) { return a.id !== b.id; }))
          .attr("class", "boundary")
          .attr("d", fauxCartoPath);
    });

    //all = op.concat(photos);
    all = photos;
    
    all = all.sort(function(a,b) {
      if(a.t < b.t)
        return -1;
      if(a.t > b.t)
        return 1;
      return 0;
    });

    var timeBounds = [all[0].t, all[all.length-1].t].map(function(t){
      var d = new Date();
      d.setTime(t*1000);
      return d.toUTCString();
    });
    console.log("First: " + timeBounds[0]);
    console.log("Last:  " + timeBounds[1]);

    var CBPoint = {
      transform : function(pt) {
        return "translate(" + fauxCartoProjection([pt.lon, pt.lat]).join(',') + ")";
      },
      vertex : function(pt) {
        return fauxCartoProjection([pt.lon, pt.lat]);
      },
      class : function(pt) {
        return pt.path ? "photo" : "op";
      },
      id : function(pt, i) {
        return "photo-" + i;
      },
      radius : function(pt) {
        return (pt.path ? 5 : 2) / scale + "px";
      },
      path : function(pt) {
        return pt.path;
      },
      click : function(pt) {
        window.open(pt.path ? "/f?path=" + pt.path : "//maps.google.com/maps?q="+pt.lat+","+pt.lon , '_blank');
      },
      photoUrl : function(pt) {
        return pt.path ? "/f?path=" + pt.path : "";
      }
    }

    var voronoiLayer = geoclip.append("svg:g")
        .attr("id", "voronoi");
    var hash = {};
    var vertices = all.map(CBPoint.vertex).map(function(v) {
      function key(v) { return v[0] + "_" + v[1]; }
      while(key(v) in hash) {
	var eps = 0.0000001;
        v[0] += eps / 2.0 - eps * Math.random();
        v[1] += eps / 2.0 - eps * Math.random();
      }
      hash[key(v)] = true;
      return v;
    });
    var left   = Math.min.apply(null, vertices.map(function(v){return v[0];}));
    var right  = Math.max.apply(null, vertices.map(function(v){return v[0];}));
    var top    = Math.min.apply(null, vertices.map(function(v){return v[1];}));
    var bottom = Math.max.apply(null, vertices.map(function(v){return v[1];}));
    console.log(left, right, bottom, top);
    //var voronoi = d3.geom.voronoi(vertices);//.map(function(cell) { return viewportBounds.clip(cell); });
    //var voronoi = d3.geom.delaunay(vertices);//.map(function(cell) { return viewportBounds.clip(cell); });
    //console.log(voronoi);
    /*
    voronoiLayer.selectAll("path")
                 .data(voronoi)
	       .enter().append("path")
	         .attr("d", function(d) { return "M" + d.join("L") + "Z"; });
    */

    waypoints.selectAll("circle")
        .data(all)
      .enter().append("svg:circle")
        .attr("cx", 0)
        .attr("cy", 0)
        .attr("r", CBPoint.radius)
        .attr("transform", CBPoint.transform)
	.attr("class", CBPoint.class)
	.attr("title", CBPoint.path)
	.attr("id", CBPoint.id)
	.on("click", CBPoint.click)
	.on("mouseover", function(d, i) {
	  d3.select('body').append('div').datum(d)
	      .attr('id', 'photo-'+i+'-popover')
	      .attr('class', 'photo-popover')
	      .style('left', '0px')
	      .style('top', '0px')
	      .style('background-image', 'url('+CBPoint.photoUrl(d)+')');
	})
	.on("mouseout", function(d, i) {
	  d3.select('#photo-'+i+'-popover').remove();
	})
	;
   
   var cartogram = d3.cartogram()
    .projection(d3.geo.mercator())
    .value(function(d) {
      return 10 + Math.random() * 2;
      //return 1;
    });
    
    var cartogramLayer = geoclip.append("svg:g")
        .attr("id", "cartogram");
    d3.json("data/us-states-segmentized.topojson", function(topology) {
      var features = cartogram(topology, topology.objects.states.geometries).features;
      console.log('features', features);
      cartogramLayer.selectAll("path")
        .data(features)
        .enter()
        .append("path")
          .attr("d", cartogram.path);
    });
    

    var zoom = d3.behavior.zoom()
      .translate([-left, -top])
      //.scale(projection.scale())
      //.scaleExtent([window.height, 8 * window.height])
      .on("zoom", zoom);

    geoclip.attr('transform', "translate("+[-left, -top].join(",")+")");
    svg.call(zoom);

    function zoom() {
      geoclip.attr('transform', 'translate('+d3.event.translate+') scale('+d3.event.scale+')');
      map.selectAll('path').style("stroke-width", strokeWidth / d3.event.scale + "px");
      
      scale = d3.event.scale;
      waypoints.selectAll('circle').data(all).attr("r", CBPoint.radius);
    }
 

  });
//});
}); // end $(document).ready()

d3.select("select").on("change", function() {
  d3.selectAll("svg").attr("class", this.value);
});



