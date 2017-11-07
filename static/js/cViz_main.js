/* global $, _, crossfilter, d3  */

(function(cViz) {
    //'use strict';
//    cViz.directory = document.getElementById("partSetName").value;
//    console.log(cViz.directory);
    // Load the initial data
//    console.log(cViz.directory);
    queue()
        //.defer(cViz.getData, "data/result.tsv")
        .defer(d3.tsv, "/static/TSV/" + cViz.directory + "/result.tsv")
        .defer(cViz.getColumnOrder, "/static/TSV/" + cViz.directory + "/objOrderEAC.tsv")
        .defer(cViz.getIdenticalObjs, "/static/TSV/" + cViz.directory + "/listsIdenticalObjs.tsv")
        .defer(cViz.getDistinctClusters, "/static/TSV/" + cViz.directory + "/listDistinctClusters.tsv")
        .await(ready);
    //cViz.getData("data/result.tsv");

    //cViz.getData("data/result.tsv", cViz.drawTable);


    function ready(error, data, objOrderEAC, listsIdenticalObjs, distinctClusters) {

        if (error) throw error;
        //console.log(data);
        //console.log(objOrderEAC);
        //console.log(listsIdenticalObjs);

        //cViz.drag = d3.drag();
        //d3.selectAll(".draggable").call(cViz.drag);
 /*       cViz.rowDrag = d3.drag()
                        .on("start", console.log("starting drag"))
                        .on("drag", console.log("dragging"))
                        .on("end", console.log("ending drag"));
                        //.on("start.render drag.render end.render", render));*/
/*
                        .on("start", dragstarted)
                        .on("drag", dragged)
                        .on("end", dragended)
                        .on("start.render drag.render end.render", render));
*/
        cViz.data.allData = data;
        //cViz.data.objOrderEAC = objOrderEAC.columns;
        cViz.data.objOrderEAC = objOrderEAC;
        cViz.data.identicalObjs = listsIdenticalObjs;
        //console.log(cViz.data.allData);
        cViz.data.clustersData = cViz.getClustersData();
        cViz.data.distinctClusters = distinctClusters;

        cViz.columnsOrder = ["ID"];
        cViz.columnsOrder = cViz.columnsOrder.concat(cViz.data.objOrderEAC);
        cViz.columnsOrder = cViz.columnsOrder.concat(["cLabel", "pLabel", "cSize"]);
        cViz.data.clustersData.columns = cViz.columnsOrder;
        //console.log(cViz.data.clustersData.columns);

        //cViz.getClustersData();
        //console.log(cViz.data.clustersData);
        cViz.prepareTable();
        cViz.onDataChange();
        //console.log(cViz.data.clustersData);

    }

//    function update(myRegion){
//
//    var newData = data.filter(function(d){ return d.region == myRegion})
}(window.cViz = window.cViz || {}));
