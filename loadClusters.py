from operator import itemgetter, attrgetter, methodcaller
import numpy as np
import distutils
#import sys
import os
import csv
import pandas as pd


# O parâmetro dirPart NECESSARIAMENTE tem que ser uma string
def loadCluster(dirPart):
    merged = pd.DataFrame()
    clustersData = pd.DataFrame(columns=["ID", "cLabel", "pLabel"]) #pd.DataFrame(columns=["cID", "cLabel", "pLabel", "cSize"])

    # dirPartitions = os.getcwd() + "/partitions-uploaded"
    dirPartitions = os.getcwd() + "/" + dirPart;
    (head, tail) = os.path.split(dirPartitions);
    expDirName = tail + '-files';

    resDir = head + '/' + expDirName # os.path.join(head, expDirName);

    if not os.path.exists(resDir):
        os.makedirs(resDir)

    allClustersLabels = []

    for diretorios in os.listdir(dirPartitions):
        clusterDir = dirPartitions + '/' + diretorios
        for filename in os.listdir(clusterDir):
            # check if there is any subdirs
            path = os.path.join(clusterDir, filename)
            if os.path.isdir(path):
               # change filenames to new path and ignore .err files
               for filename in os.listdir(path):
                   if filename.endswith('.clu'):
                       file = path + '/' + filename
            else:
                file = clusterDir + '/' + filename

            p = pd.read_csv(file, sep="\t", header=None, encoding='utf-8',
                            engine="python")
            p.columns = ["ID", "clusterLabel"]
            allClusters = set(p["clusterLabel"])

            for cluster in allClusters:
                cLabel = "c"+str(cluster)
                newCluster = cLabel+filename
                allClustersLabels.insert(len(allClustersLabels), newCluster)
                p[newCluster] = (p["clusterLabel"] == cluster).astype(int) # column with cluster membership (1/0)
                clustersData = clustersData.append({"ID": newCluster, "cLabel": cLabel, "pLabel": filename, "cSize": p[newCluster].sum()}, ignore_index=True)
            del p["clusterLabel"]

            if merged.empty:
                merged = pd.DataFrame(p)
            else:
                merged = merged.merge(p, how="outer", on="ID")

    # identify groups of objects identically grouped and remove them from the dataset
    distinct = {}
    dist2 = merged.groupby(allClustersLabels, sort=False)
    file = resDir + '/listsIdenticalObjs.tsv'

    with open(file, 'w') as f:
        out = csv.writer(f, delimiter='\t', lineterminator='\n')
        # identify the objects identically grouped in all clusters
        for name, group in dist2:
            values = group["ID"].values
            distinct[values[0]] = values
            out.writerow(values)
    repeatedObjs = merged.loc[~merged["ID"].isin(distinct.keys())]["ID"].index
    # save the lists of identically grouped objects
    merged.drop(repeatedObjs,inplace=True)

    objectsIDs = merged["ID"]
    clustersData = clustersData.set_index(["ID"])
    merged = merged.set_index(["ID"])

    mergedT = merged.transpose()
    # identifies the distinct clusters
    distinctClusters = mergedT.drop_duplicates().index

    file = resDir + '/listDistinctClusters.tsv'
    with open(file, 'w') as f:
        out = csv.writer(f, delimiter='\t', lineterminator='\n')
        out.writerow([k for k in distinctClusters])
    complete = mergedT.join(clustersData)
    complete.sort_values(by=["cSize"], ascending=False, inplace=True)

    file = resDir + '/result.tsv'
    complete.to_csv(file ,sep='\t', index_label = "ID")
    eacM = {}

    for o1 in objectsIDs:
        eacM[o1] = complete.loc[complete[o1]==1]["cSize"].sum()

    x = sorted(eacM.items(), key=itemgetter(1), reverse=True)

    file = os.path.join(resDir, 'objOrderEAC.tsv')
    file = resDir + '/objOrderEAC.tsv'

    with open(file, 'w') as f:
        out = csv.writer(f, delimiter='\t', lineterminator='\n')
        out.writerow([k for (k,v) in x])
