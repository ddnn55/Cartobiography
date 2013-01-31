var strokeWidth = 1.0
  , waypointRadius = 2.0
  , π = 3.14159265359
  ;

var all, scale = 1.0;


var projection = d3.geo.mercator()
      .scale(3500)
      .translate([1000, 700]);

function makeFauxCartoProjection(points)
{
  return d3.geo.projection(function(λ, φ) {
    return [
      λ / (2 * π),
      Math.max(-.5, Math.min(+.5, Math.log(Math.tan(π / 4 + φ / 2)) / (2 * π)))
    ];
  })
    .scale(3500)
    .translate([1000, 700]);
}


var path = d3.geo.path()
    .projection(projection);





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


d3.json("us-states.json", function(json) {
  states.selectAll("path")
      .data(json.features)
    .enter().append("svg:path")
      .attr("d", path);
});

d3.json("world-110m.json", function(error, world) {
  map.insert("path", ".graticule")
      .datum(topojson.object(world, world.objects.land))
      .attr("class", "land")
      .attr("d", path);

  map.insert("path", ".graticule")
      .datum(topojson.mesh(world, world.objects.countries, function(a, b) { return a.id !== b.id; }))
      .attr("class", "boundary")
      .attr("d", path);
});

d3.json("openpaths_davidstolarsky.json", function(op) {
  console.log(op.length + " OpenPaths waypoints");
  d3.json("photos.json", function(photos) {
    console.log(photos.length + " geotagged photos");

    // make 1d-1d-faux-cartogram projection
    var fauxCartoProjection = makeFauxCartoProjection(photos);
    var fauxCartoPath = d3.geo.path()
          .projection(fauxCartoProjection);

    all = op.concat(photos);
    
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
        return "translate(" + projection([pt.lon, pt.lat]).join(',') + ")";
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
    var zoom = d3.behavior.zoom()
      //.translate(projection.translate())
      //.scale(projection.scale())
      //.scaleExtent([window.height, 8 * window.height])
      .on("zoom", zoom);

    svg.call(zoom);

    function zoom() {
      geoclip.attr('transform', 'translate('+d3.event.translate+') scale('+d3.event.scale+')');
      map.selectAll('path').style("stroke-width", strokeWidth / d3.event.scale + "px");
      
      scale = d3.event.scale;
      waypoints.selectAll('circle').data(all).attr("r", CBPoint.radius);
    }
 

  });
});

d3.select("select").on("change", function() {
  d3.selectAll("svg").attr("class", this.value);
});


