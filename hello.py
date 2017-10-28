from flask import Flask, render_template, request, flash, redirect, url_for, jsonify
from flask_dropzone import Dropzone
from datetime import datetime
import json
from natsort import  natsorted, ns
import os
from loadClusters import *
#from script_fcs import *

app = Flask(__name__, static_folder="static")
dropzone = Dropzone(app)

# get the current folder
app.config['UPLOADED_PATH'] = os.getcwd()

# essa variável serve pra gente não ter que ficar rodando
# o servidor de novo toda hora
# por via das dúvidas dá um export nela tbm
DEBUG = True

@app.route('/')
def index():
   return render_template('index.html')

@app.route('/', methods=['GET', 'POST'])
def upload():
    if request.method == 'POST':
        if(request.form['name'] == 'dataset'):
            print("request dataset")
            id = str(datetime.now().strftime('%d-%m-%Y/%H:%M:%S'))
            dir = '/uploaded-data/' + id

            if not os.path.exists(app.config['UPLOADED_PATH'] + dir):
                os.makedirs(app.config['UPLOADED_PATH'] + dir)

            for f in request.files.getlist('file'):
                print("request files: ", request.files)
                f.save(os.path.join(app.config['UPLOADED_PATH'] + dir, f.filename))

            datasetlocation = app.config['UPLOADED_PATH'] + dir + '/' + f.filename
            # lê número de objetos do dataset
            with open(datasetlocation) as f:
                numObj = sum(1 for _ in f)-1

            # caso 1: rodar os algoritmos em cima do dataset
            if(request.form['partition'] == 'no'):
                newdirectory = False
                if not os.path.exists(app.config['UPLOADED_PATH'] + '/algResult/'):
                    os.makedirs(app.config['UPLOADED_PATH'] + '/algResult/')
                    newdirectory = True

                if(newdirectory):
                    os.makedirs(app.config['UPLOADED_PATH'] + '/algResult/' + 'results-1/')
                    resultfolder = app.config['UPLOADED_PATH'] + '/algResult/' + 'results-1/'
                else:
                    # ordena as pastas e pega o número da última pra saber qual o
                    # número da pasta nova 
                    root, dirs, files = next(os.walk(app.config['UPLOADED_PATH'] + '/algResult/'))
                    # natsorted serve para ordenar 10+
                    newdir = int((natsorted(dirs)[-1]).split("-")[1]) + 1
                    os.makedirs(app.config['UPLOADED_PATH'] + '/algResult/' + 'results-' + str(newdir))
                    # pasta para adicionar resultados tanto do clustering quanto do mocle
                    tail = 'algResult/' + 'results-' + str(newdir)
                    resultfolder = app.config['UPLOADED_PATH'] + tail

                if(request.form['basicSelected'] == 'yes' and request.form['mocleSelected'] == 'yes'):
                    print("CLUSTERING AND MOCLE SELECTED")
                    minK = int(request.form['minKBasic'])
                    maxK = int(request.form['maxKBasic'])

                    # crossover E ou P
                    tipoDistBasic = request.form['tipoDistBasic']
                    # algorithms (1, 2, 3, 4, 5, 6, 7)
                    # se composto, vem com virgula
                    alg = request.form['alg'].split(',')
                    print("alg", alg)
                    if len(alg) > 1:
                        for algnumber in alg:
                            print("algnumber: ", algnumber)
                            clustering(tipoDistBasic, numObj, minK, maxK, datasetlocation, resultfolder, int(algnumber))
                    else:
                        clustering(tipoDistBasic, numObj, minK, maxK, datasetlocation, resultfolder, int(alg[0]))

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
                    mocle(crossover, minK, maxK, datasetlocation, resultfolder + '/AllParts', resultfolder, datasetlocation, nearNeigh, numGen)
                # conferir no loadClusters como o caminho tá sendo pegado
            path = loadCluster(tail)
            #elif(request.form['name'] == 'partition'):
        else:
            print("request partition")
            flag = False
            if(request.form['mocleSelected'] == 'yes'):
                # essa aqui é a gambiarra mais feia da face da terra, mas
                # aparentemente a variável do form perde o valor??? sei lá
                flag = True
                print("mocle ok, mas ainda não tá indo pra onde deveria ir")
            id = str(datetime.now().strftime('%d-%m-%Y/%H:%M:%S'))

            dir = '/uploaded-part/' + id

            # cria diretorio uploaded na pasta atual, se já não existir
            if not os.path.exists(app.config['UPLOADED_PATH'] + dir):
                os.makedirs(app.config['UPLOADED_PATH'] + dir)

            for n in request.files.getlist('datasetfile'):
                datasetlocation = app.config['UPLOADED_PATH'] + dir
                n.save(os.path.join(datasetlocation, n.filename))
                datasetlocation += '/' + n.filename

            if not os.path.exists(app.config['UPLOADED_PATH'] + dir + '/partition'):
                os.makedirs(app.config['UPLOADED_PATH'] + dir + '/partition')

            # loop over files since we allow multiple files
            for f in request.files.getlist('file'):
                #salva arquivos na pasta atual + /uploaded
                partitionlocation = app.config['UPLOADED_PATH'] + dir + '/partition/'
                f.save(os.path.join(partitionlocation, f.filename));

            # checa numero de arquivos na pasta para saber se é o ultimo post
            qtinfolder = len([name for name in os.listdir(app.config['UPLOADED_PATH'] + dir + '/partition/') if os.path.isfile(os.path.join(app.config['UPLOADED_PATH'] + dir + '/partition', name))])

            newdirectory = False
            if not os.path.exists(app.config['UPLOADED_PATH'] + '/algResult/'):
                os.makedirs(app.config['UPLOADED_PATH'] + '/algResult/')
                newdirectory = True

            if(newdirectory):
                os.makedirs(app.config['UPLOADED_PATH'] + '/algResult/' + 'results-1/')
                # pasta para adicionar resultados tanto do clustering quanto do mocle
                resultfolder = app.config['UPLOADED_PATH'] + '/algResult/' + 'results-1/'
            else:
                # pega nome das pastas e ordena, pega a ultima pasta e adiciona mais um para criar a nova
                root, dirs, files = next(os.walk(app.config['UPLOADED_PATH'] + '/algResult/'))
                newdir = int((natsorted(dirs)[-1]).split("-")[1]) + 1
                os.makedirs(app.config['UPLOADED_PATH'] + '/algResult/' + 'results-' + str(newdir))
                # pasta para adicionar resultados tanto do clustering quanto do mocle
                resultfolder = app.config['UPLOADED_PATH'] + '/algResult/' + 'results-' + str(newdir)

            # compara numero de arquivos na pasta com numero de arquivos aceitos no dropzone
            if(qtinfolder == int(request.form['qtofdata'])):
                print("Oi! Você fez upload do partitions!");

                minK = int(request.form['minKMocle'])
                maxK = int(request.form['maxKMocle'])
                print("minK = {}, maxK = {}".format(minK, maxK))

                if(request.form['mocleSelected'] == 'yes'):
                    print("MOCLE SELECTED")
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

                # chama loadClusters.py aqui
                path = loadCluster(resultfolder)
        return jsonify(path)


#!/usr/bin/env python
#-*- coding: utf-8 -*-
# Script para criar as pastas necessárias e rodar os algoritmos de
# clusterização etc
#
import subprocess

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
    #print("tipos clustering:\ntipoDist: {}\nnumObj: {}\nminK: {}\nmaxK: {}\ndataset: {}\nexpDir:{}\nalg: {}".format(type(tipoDist), type(numObj), type(minK), type(maxK), type(dataset), type(expDir), type(alg)))
    args = ['/home/lasid/programs/clustering/./clustering', tipoDist, numObj, minK, maxK, dataset, expDir, alg]
    processo = ""

    for item in args:
        processo += str(item)
        processo += " "

    return subprocess.call(processo, shell=True)

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

    args = ['/home/lasid/programs/MOCLE-v3/./mocle', crossover, minK, maxK, dataset, popIniDir, resultDir, truePartition, nearNeigh, numGem]
    processo = ""

    for item in args:
        processo += str(item)
        processo += " "
    return subprocess.call(processo, shell=True)


## teste:: remover depoi ##
#print("teste::\n")
#clustering("E", 905, 3, 6, "./ds3c3sc6.txt", "./ds3c3sc6-E", 1)

if __name__ == '__main__':
    app.run(debug=True)
