#-*- coding: utf-8 -*-
from flask import Flask, render_template, request, flash, redirect, url_for, jsonify
from flask_dropzone import Dropzone
from datetime import datetime
import json
from natsort import  natsorted, ns
import pprint
import os, sys, stat
import shutil
from loadClusters import *
import subprocess32 as subprocess
#from script_fcs import *

app = Flask(__name__, static_folder="static")
dropzone = Dropzone(app)

# insert the static folder where you want to save the results
app.config['UPLOADED_PATH'] = '/home/cvis/'

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/visualization')
def visualization():
    return render_template('visualization.html')

@app.route('/', methods=['GET', 'POST'])
def upload():
    if request.method == 'POST':
        path = ""
	paramtext = ""
	algsnames = ['All', 'Single Link', 'Average Link', 'Centroid Link', 'Complete Link', 'SNN', 'KMeans']
	crossovernames = ['MCLA', 'HBGF']
        if(request.form['name'] == 'dataset'):
            print("request dataset")
            id = str(datetime.now().strftime('%d-%m-%Y/%H:%M:%S'))
            dir = '/uploaded-data/' + id

            if not os.path.exists(app.config['UPLOADED_PATH'] + dir):
                try:
                    original_umask = os.umask(0)
                    os.makedirs(app.config['UPLOADED_PATH'] + dir, 0777)
                finally:
                    os.umask(original_umask)


            for f in request.files.getlist('file'):
                print("request files: ", request.files)
		datasetname = f.filename
                f.save(os.path.join(app.config['UPLOADED_PATH'] + dir, f.filename))

            datasetlocation = app.config['UPLOADED_PATH'] + dir + '/' + f.filename
            # lê número de objetos do dataset
            with open(datasetlocation) as f:
                numObj = sum(1 for _ in f)-1

            # caso 1: rodar os algoritmos em cima do dataset
            if(request.form['partition'] == 'no'):
                print("request.form['partition'] == 'no'")
                newdirectory = False
                if not os.path.exists(app.config['UPLOADED_PATH'] + 'algResult/'):
                    try:
                        original_umask = os.umask(0)
                        os.makedirs(app.config['UPLOADED_PATH'] + 'algResult/', 0777)
                    finally:
                        os.umask(original_umask)
                    newdirectory = True

                if(newdirectory):
                    try:
                        original_umask = os.umask(0)
                        os.makedirs(app.config['UPLOADED_PATH'] + 'algResult/' + 'cvis-1', 0o2770)
                    finally:
                        os.umask(original_umask)
                    resultfolder = app.config['UPLOADED_PATH'] + 'algResult/' + 'cvis-1'
                else:
                    # ordena as pastas e pega o número da última pra saber qual o
                    # número da pasta nova
                    root, dirs, files = next(os.walk(app.config['UPLOADED_PATH'] + 'algResult/'))
                    # natsorted serve para ordenar 10+
                    newdir = int((natsorted(dirs)[-1]).split("-")[1]) + 1
                    resultfolder = app.config['UPLOADED_PATH'] + 'algResult/' + 'cvis-' + str(newdir)
                    try:
                        original_umask = os.umask(0)
                        os.makedirs(resultfolder, 0o2770)
                    finally:
                        os.umask(original_umask)
		

                if(request.form['basicSelected'] == 'yes'):
                    print("CLUSTERING SELECTED")
                    minK = int(request.form['minKBasic'])
                    maxK = int(request.form['maxKBasic'])

                    # crossover E ou P
                    tipoDistBasic = request.form['tipoDistBasic']
                    # algorithms (1, 2, 3, 4, 5, 6, 7)
                    # se composto, vem com virgula
                    alg = request.form['alg'].split(',')

                    paramtext += 'Chosen algorithms: '
                    if len(alg) > 1:
                        dest = resultfolder+'/allPart'
                        try:
                            original_umask = os.umask(0)
                            os.makedirs(dest, 0o2770)
                        finally:
                            os.umask(original_umask)
                        for algnumber in alg:
                            resultAlg = resultfolder + '/' + algnumber
                            clustering(tipoDistBasic, numObj, minK, maxK, datasetlocation, resultAlg, int(algnumber))

                            print("resulAlg: ", resultAlg)
                            for files in os.listdir(resultAlg+'/allPart'):
                                source = os.path.join(resultAlg+'/allPart',files)
                                shutil.copy(source, dest)
                            print("RESULT FOLDER GENERATE BASIC PARTITIONS: {}".format(resultfolder))
                            paramtext += str(algsnames[int(algnumber)-1])+'; '
			    
	            else:
                        clustering(tipoDistBasic, numObj, minK, maxK, datasetlocation, resultfolder, int(alg[0]))
                        print("RESULT FOLDER GENERATE BASIC PARTITIONS: {}".format(resultfolder))
			paramtext += str(algsnames[int(alg[0])-1])
		    paramtext += '\n\nSimilarity/Dissimilarity: '+str(tipoDistBasic)+'\n'
                    paramtext += 'Min number of clusters: '+str(minK)+'\nMax number of clusters: '+str(maxK)+'\n'

                if(request.form['basicSelected'] == 'yes' and request.form['mocleSelected'] == 'yes'):
                    print("BASIC + MOCLE SELECTED")
                    # crossover
                    # tratar: se M = 1 ou se B = 2
                    if(request.form['tipoDistMocle'] == 'M'):
                        crossover = 1
                    else:
                        crossover = 2
                    numGen = int(request.form['numGen'])
                    nearNeigh = int(request.form['nearNeigh'])
                    minK = int(request.form['minKMocle'])
                    maxK = int(request.form['maxKMocle'])

                    trueP = os.listdir(resultfolder+'/allPart')
                    print("antes de tudo dar errado:")
                    mocle(crossover, minK, maxK, datasetlocation, resultfolder + '/allPart', resultfolder + '/allPart-mocle', resultfolder + '/allPart/'+trueP[0], nearNeigh, numGen)
		    permissao(resultfolder)
                    path = loadCluster(resultfolder + '/allPart-mocle/solutionPopulation', 2)
		    paramtext += '\nCrossover: '+crossovernames[crossover-1]
		    paramtext += '\nMin k of the consensus partition: '+str(request.form['minKMocle'])
		    paramtext += '\nMax k of the consensus partition: '+str(request.form['maxKMocle'])
		    paramtext += '\nNumber of generations: '+str(numGen)
		    paramtext += '\nNearest neighbours: '+str(nearNeigh)+'%\n'
                else:
		    pathcompleto = resultfolder + '/allPart'
		    path = loadCluster(pathcompleto, 1)
                
                # conferir no loadClusters como o caminho tá sendo pegado
                (head, tail) = os.path.split(path)
               	viszipado = shutil.make_archive(tail+'-vis', 'zip', str(path))
                shutil.move(viszipado, path)
		nomeparam = path + '/' + tail + '-parameters.txt'
                param = open(nomeparam, 'w+')
                param.write('You chose to generate partitions\n\nBasic selected: '+request.form['basicSelected']+'\nMocle selected: '+request.form['mocleSelected']+'\n\nUploaded dataset:'+datasetname+'\n\n')
		param.write(paramtext)
		param.close()
                partzipado = shutil.make_archive(tail+'-partition', 'zip', str(resultfolder))
                shutil.move(partzipado, path)
               	path = './static/resultados/'+tail
	        print("path loadCluster = ", path)
                return jsonify(path)

        elif request.form['name'] == 'partition':
            print("request partition")
            #id = str(datetime.now().strftime('%d-%m-%Y/%H:%M'))

            id = 'cvis-1'
            dir = '/uploaded-part/' + id
            dirAtual = app.config['UPLOADED_PATH']
            novoDir = dirAtual + dir

            if not os.path.exists(dirAtual + dir):
                try:
                    original_umask = os.umask(0)
                    os.makedirs(novoDir, 0777)
                finally:
                    os.umask(original_umask)
            else:
                root, dirs, files = next(os.walk(dirAtual + '/uploaded-part/'))
                print("natsorted(dirs) ", natsorted(dirs))
                newdir = int((natsorted(dirs)[-1]).split("-")[1]) + 1
                novoDir = dirAtual +'/uploaded-part/' + 'cvis-' + str(newdir)
                print("diretório criado: ", novoDir)
                try:
                    original_umask = os.umask(0)
                    os.makedirs(novoDir, 0777)
                finally:
                    os.umask(original_umask)

            for n in request.files.getlist('datasetfile'):
                datasetlocation = novoDir
                datasetname = n.filename
                n.save(os.path.join(datasetlocation, n.filename))
                datasetlocation += '/' + n.filename

            if not os.path.exists(novoDir + '/partition'):
                try:
                    original_umask = os.umask(0)
                    os.makedirs(novoDir+'/partition', 0o2770)
                finally:
                    os.umask(original_umask)

            keys = [k for k in request.files if k.startswith('file')]
            #print(keys)

            # loop over files since we allow multiple files
            for name in keys:
                f = request.files[name]
                #salva arquivos na pasta atual + /uploaded
                partitionlocation = novoDir + '/partition'
                f.save(os.path.join(partitionlocation, f.filename));

            # checa numero de arquivos na pasta para saber se é o ultimo post
            qtinfolder = len([name for name in os.listdir(novoDir + '/partition/') if os.path.isfile(os.path.join(novoDir + '/partition', name))])

            newdirectory = False
            if not os.path.exists(app.config['UPLOADED_PATH'] + '/algResult/'):
                try:
                    original_umask = os.umask(0)
                    os.makedirs(app.config['UPLOADED_PATH']+'/algResult/', 0777)
                finally:
                    os.umask(original_umask)
                newdirectory = True

            if(newdirectory):
                try:
                    original_umask = os.umask(0)
                    os.makedirs(app.config['UPLOADED_PATH']+'/algResult/cvis-1', 0o2770)
                finally:
                    os.umask(original_umask)
                # pasta para adicionar resultados tanto do clustering quanto do mocle
                resultfolder = app.config['UPLOADED_PATH'] + '/algResult/' + 'cvis-1'
            else:
                # pega nome das pastas e ordena, pega a ultima pasta e adiciona mais um para criar a nova
                root, dirs, files = next(os.walk(app.config['UPLOADED_PATH'] + '/algResult/'))
                newdir = int((natsorted(dirs)[-1]).split("-")[1]) + 1
                try:
                    original_umask = os.umask(0)
                    os.makedirs(app.config['UPLOADED_PATH']+'/algResult/cvis-' + str(newdir), 0o2770)
                finally:
                    os.umask(original_umask)

                resultfolder = app.config['UPLOADED_PATH'] + '/algResult/' + 'cvis-' + str(newdir)

            # compara numero de arquivos na pasta com numero de arquivos aceitos no dropzone
            if(qtinfolder == int(request.form['qtofdata'])):
                print("Oi! Você fez upload do partitions!");

                if(request.form['mocleSelected'] == 'yes'):
                    print("MOCLE + UPLOAD SELECTED")
                    minK = int(request.form['minKMocle'])
                    maxK = int(request.form['maxKMocle'])
                    print("minK = {}, maxK = {}".format(minK, maxK))

                    # tratar: se M = 1 ou se B = 2
                    if(request.form['tipoDistMocle'] == 'M'):
                        crossover = 1
                    else:
                        crossover = 2
                    # number of generations
                    numGen = request.form['numGen']
                    # nearest neighbours
                    nearNeigh = request.form['nearNeigh']
                    mocle(crossover, minK, maxK, datasetlocation, partitionlocation, resultfolder, datasetlocation, nearNeigh, numGen)
                    permissao(resultfolder)
                    path = loadCluster(resultfolder+'/solutionPopulation', 2)
		    (head, tail) = os.path.split(path)
		    viszipado = shutil.make_archive(tail+'-vis', 'zip', str(path))
                    shutil.move(viszipado, path)
		    partzipado = shutil.make_archive(tail+'-partition', 'zip', str(resultfolder))
	            shutil.move(partzipado, path)
                else:
                    permissao(partitionlocation)
                    path = loadCluster(partitionlocation, 1)
		    (head, tail) = os.path.split(path)
	            viszipado = shutil.make_archive(tail+'-vis', 'zip', str(path))
                    shutil.move(viszipado, path)
                    partzipado = shutil.make_archive(tail+'-partition', 'zip', str(partitionlocation))
	            shutil.move(partzipado, path)

        	(head, tail) = os.path.split(path)
		nomeparam = path + '/' + tail + '-parameters.txt'
                param = open(nomeparam, 'w+')
                param.write('You chose to upload partitions\n\nMocle selected: '+request.form['mocleSelected']+'\n\nUploaded dataset: '+datasetname+'\n\n')
                param.write(paramtext)
                param.close()
                path = './static/resultados/'+tail
                print("path loadCluster: ", path)

                return jsonify(path)
    return "Erro"

def permissao(algResult):
    for root, dirs, files in os.walk(algResult, topdown=False):
        print("mudando permissão de {}".format(root))
        os.chmod(root, 0777)
        for name in files:
            print("mudando permissão de {}".format(os.path.join(root, name)))
            os.chmod(os.path.join(root,name), 0o777)



#!/usr/bin/ python
#-*- coding: utf-8 -*-
# Script para criar as pastas necessárias e rodar os algoritmos de
# clusterização
#

def clustering(tipoDist, numObj, minK, maxK, dataset, expDir, alg):
    """
    Parâmetros:
    ----------
        tipoDist :  char
            E para distância Euclidiana
            P para correlação de Pearson
        numObj  :   int
            número de objetos
        minK    :   int
            número mínimo de clusters
            número máximo de clusters
        dataset :   string
            caminho absoluto para o arquivo com o dataset
        expDir  :   string
            caminho absoluto para o diretório com os resultados
        alg     :   int
            algoritmo de clusterização que será rodado
            1 - todos os algoritmos
            2 - SL
            3 - AL
            4 - CeL
            5 - CoL
            6 - SNN
            7 - KM
    Retorno:
    --------
        todos os arquivos .clu são gerados em subdiretórios, identificados pelo
        algoritmo selecionado, na pasta expDir
    """
    args = ['/home/lasid/programs/clustering/./clustering', str(tipoDist), str(numObj), str(minK), str(maxK), dataset, expDir, str(alg)]
    processo = ""

    for item in args:
        processo += str(item)
        processo += " "

    print("processo clustering: ", processo)
    #return subprocess.check_call(processo, shell=True)
    return subprocess.check_call(processo, shell=True)



def mocle(crossover, minK, maxK, dataset, popIniDir, resultDir, truePartition, nearNeigh, numGem):
    """
    Parâmetros
    ---------
        crossover  :  int
            1 - mcla
            2 - bipartite
        minK, maxK :  int
        dataset    :  string
            caminho absoluto pro arquivo de dataset
        popIniDir  :  string
            caminho absoluto pro arquivo de população inicial
        resultDir  :  string
            caminho absoluto pro arquivo de resultados
        truePartition : fake
            é inútil
    """
    """
    Usage: ./mocle <Crossover Type: 1- mcla or 2 - bipartite> <minK> <maxK> <Dataset> <Initial Population Directory> <Results Directory> <True Partition (can be a fake one if not known)> [L - Percentage of the number of neighbors for calculating the connectivity - default = 5%] [G - Number of generations - defauld = 100]
    """
    args = ["/home/lasid/programs/MOCLE-v3/./mocle", crossover, minK, maxK, dataset, popIniDir+'/', resultDir+'/', truePartition, nearNeigh, numGem]
    processo = ""

    for item in args:
        processo += str(item)
        processo += " "

    print("processo mocle: ", processo)
    #return subprocess.call(processo, shell=True)
    return subprocess.check_call(processo, shell=True)


if __name__ == '__main__':
    app.run(debug=True)
    app.config['TRAP_BAD_REQUEST_ERRORS'] = True
