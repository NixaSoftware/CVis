(function(cViz) { //Self-Invoking Function
    //'use strict';
    cViz.data = {}; // the main data object

    cViz.getData = function(file, callback){
        d3.tsv(file,
              function(error, data) {
                  //console.log(data);

                  if(error){
                    return callback(error);
                  }

                  callback(data);
              }).row(function(d) {
    // for each row of the data, create an object with these properties...
    return {
      time: d3.time.format('%Y/%m/%d %H:%M:%S').parse(d.Time),
      conditions: d.Conditions,
      temperature: +d.Temperature,
      humidity: d.Humidity,
      windDir: d.WindDir,
      windDeg: +d.WindDeg,
      pressure: +d.Pressure
    };
  })
  .get(function(error, csv) {
    if (!error) {
      // lets say you want to log all the temperatures to the console
      csv.forEach(function(d,i) {
        var theTime = d3.time.format('%I:%M %p')(d.time);
        console.log('The temperature at', theTime, 'was', d.temperature, 'degrees.');
      });
    } else {
      // handle error
    }
  });
    };

/*// nao funciona
    cViz.getData = function(file, callback){
        d3.tsv(file,
              function(error, data) {
                  console.log(data);

                  if(error){
                    return callback(error);
                  }

                  if('_items' in data){
                    callback(data._items);
                  }
                  else{
                    callback(data);
                  }
                  //cViz.drawTable(data);
              });
    };*/


/*
    cViz.getData = function(file){
        d3.tsv(file,
              function(error, data) {
                  if (error) throw error;
                  //cViz.data = data;
                  //var x= d3.tsvParseRows(data);
                  console.log(data);
                  return data;
              });
    };
*/

    cViz.getIdenticalObjs = function(file, callback){
        d3.text(file, function(error, data){
            if (error) throw error;
            var row = d3.csvParseRows(data);
            cViz.nObjects = 0;
            objs = [];
            row.forEach( function(r){
                //console.log(r);
                rowArray = r[0].split("\t");
                //console.log(rowArray);
                objs[rowArray[0]] = rowArray;
                cViz.nObjects = cViz.nObjects + rowArray.length;
                //objs.splice(fim do array, 0, rowArray);
            });
           //objs[i] = array com todos os objetos identicos
            //console.log(data);
            //console.log(cViz.nObjects);
            callback(null, objs);
        });
    }

    cViz.getDistinctClusters = function(file, callback){
        d3.text(file, function(error, data){
            if (error) throw error;
            var row = d3.csvParseRows(data);
            //cViz.nObjects = 0;
            distObjs = [];
            row.forEach( function(r){
                //console.log(r);
                distObjs = r[0].split("\t");
                //console.log(rowArray);
                //objs[rowArray[0]] = rowArray;
                //cViz.nObjects = cViz.nObjects + rowArray.length;
                //objs.splice(fim do array, 0, rowArray);
            });
           //objs[i] = array com todos os objetos identicos
            //console.log(distObjs);
            //console.log(cViz.nObjects);
            callback(null, distObjs);
        });
    }

    cViz.getColumnOrder = function(file, callback){
        d3.tsv(file, function(error, data){
            if (error) throw error;
            //console.log(data);
            //var cols = d3.csvParseRows(data);
            //var cols = data.split("\t")
            var cols = data.columns;
            //console.log(data);
            //console.log(cols);
            callback(null, cols);
        });
    }

    cViz.getClustersData = function(){
        //var objectsValues = d3.entries(data).filter(function(d,i) { return data[i] !== "ID"; }); // obtain the id of the objects
        cViz.allDataLabels = cViz.data.allData.columns;
        cViz.objectsLabels = cViz.data.allData.columns.filter(function(key){
            return key !== "ID" && key !== "cLabel" && key !== "pLabel" && key !== "cSize";});
        cViz.pLabel = cViz.data.allData.map(function(d){
            return d["pLabel"];});

/*
        cViz.pLabel = d3.map(cViz.data.allData, function(d){
            return d["pLabel"];}) //d3.values(d).filter(function(key){return key == "pLabel";})})
*/

        cViz.colorScale = d3.scaleOrdinal()
            //.range(["red", "blue", "green"])
            .range(colorbrewer.Set3[12])
            .domain(d3.extent(cViz.data.allData, function(d){ return d["pLabel"]; }));
            //.domain(["p1.clu", "p2.clu", "p3.clu"]); //  cViz.pLabel);


//        console.log(cViz.pLabel);
//        console.log(cViz.allDataLabels);
//        console.log(cViz.objectsLabels);
        //console.log(cViz.clustersLabels);
        //cViz.clustersData = d3.values(cViz.data.allData).filter(function(key){return key !== "ID";});

  /*     cViz.clustersSize = d3.map(cViz.data.allData, function(d){
            var count = 0;
            cViz.clustersLabels.forEach(function(c){
                console.log("d -- " + d[c]);
                if (d[c] == "1") count++;
            });
            console.log("size ----- "+count);
            return count;
       });
*/
/*
       // calculates the size of each cluster
       cViz.data.allData.forEach(function(d){
            d["size"] = 0;
            cViz.clustersLabels.forEach(function(c){
                //console.log("d -- " + d[c]);
                if (d[c] == "1") d["size"]++;
            });
            //console.log("size ----- "+count);
            //return count;
       });
       //console.log(cViz.data.allData);
*/

 /*       cViz.clustersData = d3.map(cViz.data.allData, function(d){
            return d3.values(d).filter(function(key){return key !== "ID";})})
*/
       //cViz.data.clustersData =
       cViz.data.allData.map(function(d){

            //var obj = d3.values(d).map(function(key){console.log("key"+key);})
            //console.log(obj);
            //if key !== "ID" return d[key];
            var cData = {};
            cData["ID"] = d["ID"];
            //cData["identical"] = cViz.data.identicalObjs[c];
           cViz.objectsLabels.forEach(function(c){
                cData[c] = d[c];
            })

            //console.log(cData);
            return cData;
       });
       //console.log(cViz.data.clustersData);

        cViz.data.Info = cViz.data.allData.map(function(d){
            return {
                "ID": d["ID"],
                "cLabel": d["cLabel"],
                "pLabel": d["pLabel"],
                "cSize": +d["cSize"]
            }
        });

        //console.log(cViz.data.Info);



/*
            {
                i:i,
                values: d3.maps()
               //d3.values(d).filter(function(key){return key !== "ID";})
            }})
*/


        //return cViz.data.clustersData;
        return cViz.data.allData;
/*
cViz.clustersData = cViz.data.allData.columns.map(function(key){
    return {
        key: key,
        values: cViz.data.allData.map(function(d){
            return
        })
    }
});
*/

/*
  var cities = data.columns.slice(1).map(function(id) {
    return {
      id: id,
      values: data.map(function(d) {
        return {date: d.date, temperature: d[id]};
      })
    };
  });
*/


/*
            cViz.clustersLabels.forEach(function(c){
                console.log(d[c]);
                return d[c];})
            });
*/

/*
      // This maps the data of the CSV so it can be easily accessed by
    // the ID of the municipality, for example: dataById[c1_p1]
    dataById = d3.nest()
            .key(function(d) { return d.ID; })
            .rollup(function(d) { return d[0]; })
            .map(cViz.data.allData);
*/

/*

        var clustersData = cViz.data.allData.map(function(){
        if key(i) }
        }
*/

        /*console.log(dataById);
        console.log(cViz.clustersLabels);*/
        //console.log(cViz.data.allData);
        //var data = d3.values(cViz.data.allData).filter(function(d,i) {console.log(i); return d !== "ID"; });
   //     return cViz.data.allData.filter(function(key) { return key !== "ID"; });
        //var clusters = cViz.data;
    }

   cViz.prepareTable = function(){

        //cViz.clustersTableWidth = parseInt(d3.select("#clustersTable").style("width")); //to calculate the width of each cell proportional to the number of objects it represents
        var table = d3.select("#clustersTable")
                        .append("table").attr("class", "w3-table");
//                        .attr("class", "sar-table")
//		                .attr("id", "constrainTable handlerTable");
        table.append("thead")
        d3.select("thead").append("tr");
        table.append("tbody");
        document.getElementById("maxCSize").value = cViz.nObjects;
        cViz.clustersTableWidth = parseInt(d3.select("#clustersTable").style("width"));
        //cViz.clustersTableWidth = parseInt(d3.select("#cViz").style("width"));
//        cViz.clustersTableWidth = document.documentElement.clientWidth;
        console.log(cViz.clustersTableWidth);



//        cViz.columnsOrder = ["ID"];
//        cViz.columnsOrder = cViz.columnsOrder.concat(cViz.data.objOrderEAC);
//        cViz.columnsOrder = cViz.columnsOrder.concat(["cLabel", "pLabel", "cSize"]);
   }

   cViz.onDataChange = function(){
        //var data = cViz.getData();

        //$('input[name="maxSize"]').value = cViz.nObjects;
        //console.log(d3.select("input[name=maxSize]").value);
        //console.log(document.getElementByName("maxSize").value);
//        d3.select("maxSize").value = cViz.nObjects;

        //console.log((d3.select("#allClusters")).value);
        //console.log(document.getElementById("maxCSize").value);
        //if (d3.select("#allClusters").value == "allClu")



//        if ($('input[name="allClusters"]:checked').val() == "allClu")
//            cViz.data.filteredClustersData = cViz.data.clustersData;
//        else
//            cViz.data.filteredClustersData = cViz.data.clustersData.filter(function(d){
//                return parseInt(d["cSize"]) > 200;
//            });
//
        var min =  parseInt(document.getElementById("minCSize").value);
        var max =  parseInt(document.getElementById("maxCSize").value);
        cViz.data.filteredClustersData = cViz.data.clustersData.filter(function(d){
                var cs = parseInt(d["cSize"]);
                if(d3.select('input[name="allClusters"]:checked').property("value") == "distinctClu"){
                    //console.log(d["ID"]);
                    //console.log(cViz.data.distinctClusters.indexOf(d["ID"]));
                    return (cs >= min && cs <= max && cViz.data.distinctClusters.indexOf(d["ID"])!== -1)
                }
                else
                    return (cs >= min && cs <= max);
        });

        cViz.data.filteredClustersData.columns = cViz.data.clustersData.columns;
        d3.select(".nClusters").text(cViz.data.filteredClustersData.length);
        d3.select(".nPartitions").text(d3.set(cViz.data.filteredClustersData.map(function(d){return d["pLabel"];})).size());
        //a = d3.set(cViz.data.filteredClustersData.map(function(d){return d["pLabel"];})).size();

        cViz.drawTable(cViz.data.filteredClustersData);

//        $('#constrainTable').dragtable({dragaccept:'.accept',dragHandle:'.dragHandle'});
   }



/*
    function dragstarted() {
      circles.splice(circles.indexOf(d3.event.subject), 1);
      circles.push(d3.event.subject);
      d3.event.subject.active = true;
    }

    function dragged() {
      d3.event.subject.x = d3.event.x;
      d3.event.subject.y = d3.event.y;
    }

    function dragended() {
      d3.event.subject.active = false;
    }


                        .on("dragstart", function(d) {
        	trigger = d3.event.sourceEvent.target.className.baseVal;

    		if (trigger == "label") {
	        	d3.selectAll(".cellrow").attr("opacity", 1);
				dragging[d[0].y] = y(d[0].y);
				// Move the row that is moving on the front
				sel = d3.select(this);
				sel.moveToFront();
			}
        })
        .on("drag", function(d) {
        	// Hide what is in the back
        	if (trigger == "label") {
	        	d3.selectAll(".cellcolumn").attr("opacity", 0);
	          	dragging[d[0].y] = Math.min(height, Math.max(0, d3.event.y));
	          	orders.name.sort(function(a, b) { return position(a) - position(b); });
	          	y.domain(orders.name);
	          	d3.selectAll(".row").attr("transform", function(d, i) {
	        		return "translate(0," + position(d[0].y) + ")";
	        	});
	        }
        })
        .on("dragend", function(d) {
        	if (trigger == "label") {
	          	delete dragging[d[0].y];
	          	transition(d3.select(this)).attr("transform", "translate(0," + y(d[0].y) + ")");
	          	d3.selectAll(".column").each(function(d) {
	            	d3.select(this).selectAll(".cellcolumn").attr("x", function(d) {
	        			return -y(d.y)-90; });
	          	});
	        }
        })
    );

*/



}) (window.cViz = window.cViz || {});


//http://stackoverflow.com/questions/1312236/how-do-i-create-an-html-table-with-fixed-frozen-left-column-and-scrollable-body
