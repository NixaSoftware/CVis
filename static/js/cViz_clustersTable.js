(function(cViz) {
    //'use strict';
    cViz.drawTable = function(data){

        var sortAscending = true;

        var div = d3.select("body").append("div")
            .attr("class", "tooltip")
            .style("opacity", 0);

        //console.log(data.columns);
        // create the table header
//        newColumns = ["ID"];
//        newColumns = newColumns.concat(cViz.data.objOrderEAC);
//        newColumns = newColumns.concat(["cLabel", "pLabel", "cSize"]);
//        data.columns = newColumns;
        //console.log(d3.values(cViz.data.objOrderEAC))
        var thead = d3.select("thead").select("tr").selectAll("th").data(data.columns);

        // enter
        thead.enter().append("th").merge(thead)
            .attr("class", function(d){
                if (cViz.objectsLabels.indexOf(d)!== -1)
                    return "header"; //return "header accept";
                else
                    return "header other " + d;
            })
            .style("width", function(d){
                //console.log(d3.select("td").nodes());
                //console.log(d);
                //d3.selectAll(".mYc.a101")
                if (cViz.objectsLabels.indexOf(d)!== -1){
                    //console.log(cViz.data.identicalObjs[d]);
                    //console.log(cViz.data.identicalObjs[d].length);
                    //cViz.clustersTableWidth = cViz.clustersTableWidth - d3.selectAll(".other").nodes;
                    //console.log(d3.selectAll(".other").nodes());
                    //console.log(cViz.clustersTableWidth);

                    //cellSize = cViz.clustersTableWidth * cViz.data.identicalObjs[d].length / cViz.nObjects;
                    //return cellSize  + "px";

                    cellSize = cViz.data.identicalObjs[d].length / cViz.nObjects * 100;
                    //console.log(cellSize);
                    return cellSize  + "%"
                }
//                else {
//                    return "5500px";
//                }
                })
            //.style("height", "50px")
            .on("mouseover", function(d) {
                if (cViz.objectsLabels.indexOf(d)!== -1) {
                    div.transition()
                     .duration(200)
                     .style("opacity", 1);
                    div.html(function(){
                            tooltipText = cViz.data.identicalObjs[d].length;
                             if (cViz.data.identicalObjs[d].length > 1)
                                tooltipText = tooltipText + " objects: ";
                             else
                                tooltipText = tooltipText + " object: ";
                             tooltipText = tooltipText + cViz.data.identicalObjs[d];
                            return tooltipText;
                        //cViz.data.identicalObjs[d])
                    })
                    .style("left", (d3.event.pageX) + "px")
                    .style("top", (d3.event.pageY - 28) + "px");
                }
            })
            .on("mouseout", function(d) {
                div.transition()
                 //.duration(500)
                 .style("opacity", 0);
                })
            .on('click', function (d) {
                if (cViz.objectsLabels.indexOf(d)!== -1)
                   rows.sort(function(a, b) {
                        return b[d] < a[d] ? -1 : b[d] > a[d] ? 1 : b[d] >= a[d] ? 0 : NaN;
                     });
                else {
                   if (sortAscending) {
                     rows.sort(function(a, b) {
                        if (d == "cSize") {
                            ad = parseInt(a[d]);
                            bd = parseInt(b[d]);
                        }
                        else {
                            ad = a[d];
                            bd = b[d];
                        }
                        return ad < bd ? -1 : ad > bd ? 1 : ad >= bd ? 0 : NaN;
                        //return a[d] < b[d] ? -1 : a[d] > b[d] ? 1 : a[d] >= b[d] ? 0 : NaN;
                     });
                     sortAscending = false;
                     //this.className = 'aes';
                   } else {
                     rows.sort(function(a, b) {
                                             if (d == "cSize") {
                            ad = parseInt(a[d]);
                            bd = parseInt(b[d]);
                        }
                        else {
                            ad = a[d];
                            bd = b[d];
                        }
                        return bd < ad ? -1 : bd > ad ? 1 : bd >= ad ? 0 : NaN;
                        //return b[d] < a[d] ? -1 : b[d] > a[d] ? 1 : b[d] >= a[d] ? 0 : NaN;
                     });
                     sortAscending = true;
                    // this.className = 'des';
                   }
                }
            })
            .text(function(d, i){
                if (cViz.objectsLabels.indexOf(d)!== -1) return "b" + i;
                else return d;
            });
            // to add handle for dragging columns
//            .filter(function(d){ return cViz.objectsLabels.indexOf(d)!== -1; }).append("div").attr("class", "dragHandle");

        // exit
        thead.exit().remove();


        // create rows
        var rows = d3.select("tbody").selectAll("tr").data(data);

        rows.exit().remove();
        rows = rows.enter().append("tr").merge(rows)
            .attr("class", function(c){
                return c["pLabel"];
            })
            //.style("max-height", "10px")
            .style("background-Color", function(c) {
                return  cViz.colorScale(c["pLabel"]);
            });
//            .on('click', function (d) {
//                       //thead.attr('class', 'header');
//                   //if (cViz.objectsLabels.indexOf(data.columns[i])!== -1)
//                   if (cViz.objectsLabels.indexOf(d)!== -1)
//                       rows.sort(function(a, b) {
//                            return b[d] < a[d] ? -1 : b[d] > a[d] ? 1 : b[d] >= a[d] ? 0 : NaN;
//                         });
//            });

        //rows.order();

        // cells
        var cells = rows.selectAll("td")
            .data(function(d) {
                        //console.log("oiiiii");
                        //console.log(d);
                        //https://jsfiddle.net/ud123e6k/ --- exemplo de como mostrar coluna em uma ordem especifica
                        //return d3.values(d);
                        //return d3.permute(d, newColumns);
                        return d3.permute(d, data.columns);
                        //return data.columns.map(function(m) { return d[m]; });
            });

        cells.exit().remove();

        cells = cells.enter().append("td").merge(cells)
            .attr("class", function(d, i) {
                if (cViz.objectsLabels.indexOf(data.columns[i])!== -1)
                    if (d3.values(d) == "1") {
                        return "cMember " + data.columns[i];
                    }
                    else return "cNonMember "  + data.columns[i];
                else return "other "  + data.columns[i];
            })
            .text(function(d, i) {
                if (cViz.objectsLabels.indexOf(data.columns[i])!== -1) return ""
                else return d;
            });

    }

})     (window.cViz = window.cViz || {});
