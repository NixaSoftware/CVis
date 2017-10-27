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
    print("UPLOAD")
    if request.method == 'POST':
        print("POST")
        if(request.form['name'] == 'dataset'):
            print("REQUEST = DATASET")
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
                tail = '/algResult' + 'results-' + str(newdir)
                resultfolder = app.config['UPLOADED_PATH'] + tail

            if(request.form['basicSelected'] == 'yes'):
                print("CLUSTERING SELECTED")
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

            if(request.form['mocleSelected'] == 'yes'):
                # crossover
                # tratar: se M = 1 ou se B = 2
                print("MOCLE SELECTED")
                if(request.form['tipoDistMocle'] == 'M'):
                    crossover = 1
                else:
                    crossover = 2
                numGen = int(request.form['numGen'])
                nearNeigh = int(request.form['nearNeigh'])
                minK = int(request.form['minKMocle'])
                maxK = int(request.form['maxKMocle'])
                mocle(crossover, minK, maxK, datasetlocation, resultfolder + '/AllParts', resultfolder, datasetlocation, nearNeigh, numGen)

            # depois de todo o processamento, monta a pasta de visualização e
            # xablau
            # conferir esse parâmetro dps
            path = loadCluster(tail)
        # segundo caso: dataset + partição (sem mocle)
        # nesse caso, vai direto pra visualização dos parâmetros
        elif(request.form['partition'] == 'yes'):
            print("REQUEST PARTITION")
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
                partitionlocation = app.config['UPLOADED_PATH'] + dir + '/partition/'
                f.save(os.path.join(partitionlocation, f.filename));

            # checa numero de arquivos na pasta para saber se é o ultimo post
            qtinfolder = len([name for name in os.listdir(app.config['UPLOADED_PATH'] + dir + '/partition/') if os.path.isfile(os.path.join(app.config['UPLOADED_PATH'] + dir + '/partition', name))])

            # compara numero de arquivos na pasta com numero de arquivos aceitos no dropzone
            if(qtinfolder == int(request.form['qtofdata'])):
                print("Oi! Você fez upload do partitions!");

                if(request.form['mocle'] == 'no'):
                    # teoricamente não precisa criar uma pasta de algResult pq não vai
                    # ter processamento nenhum dos dados a não ser pelo loadCluster,
                    # então acho que é ok só deixar assim:
                    path = loadCluster(partitionlocation)
                else:
                    print("REQUEST MOCLE")
                    # caso de o usuário ter selecionado upload de partição + mocle
                    newdirectory = False
                    if not os.path.exists(app.config['UPLOADED_PATH'] + '/algResult/'):
                        os.makedirs(app.config['UPLOADED_PATH'] + '/algResult/')
                        newdirectory = True

                    if(newdirectory):
                        os.makedirs(app.config['UPLOADED_PATH'] + '/algResult/' + 'results-1/')
                        resultfolder = app.config['UPLOADED_PATH'] + '/algResult/' + 'results-1/'
                    else:
                        root, dirs, files = next(os.walk(app.config['UPLOADED_PATH'] + '/algResult/'))
                        newdir = int((natsorted(dirs)[-1]).split("-")[1]) + 1
                        os.makedirs(app.config['UPLOADED_PATH'] + '/algResult/' + 'results-' + str(newdir))
                        resultfolder = app.config['UPLOADED_PATH'] + '/algResult/' + 'results-' + str(newdir)

                        minK = int(request.form['minK'])
                        maxK = int(request.form['maxK'])

                        if(request.form['mocleSelected'] == 'yes'):
                            # tratar: se M = 1 ou se B = 2
                            if(request.form['tipoDistMocle'] == 'M'):
                                crossover = 1
                            else:
                                crossover = 2

                            numGen = request.form['numGen']
                            nearNeigh = request.form['nearNeigh']

                            mocle(crossover, minK, maxK, datasetlocation, partitionlocation, resultfolder, datasetlocation, nearNeigh, numGem)

                        path = loadCluster(resultfolder)

        return jsonify(path)

#            if(request.form['partition'] == 'yes'):
#                pass
#            else:
#                #pega data e hora atual
#                id = str(datetime.now().strftime('%d-%m-%Y/%H:%M:%S'))
#
#                dir = '/uploaded-data/' + id
#                # cria diretorio uploaded na pasta atual, se já não existir
#                if not os.path.exists(app.config['UPLOADED_PATH'] + dir):
#                    os.makedirs(app.config['UPLOADED_PATH'] + dir)
#
#                for f in request.files.getlist('file'):
#                    # print para verificar erros nos arquivos
#                    print(request.files)
#                    #salva arquivos na pasta atual + /uploaded
#                    f.save(os.path.join(app.config['UPLOADED_PATH'] + dir, f.filename))
#
#                datasetlocation = app.config['UPLOADED_PATH'] + dir + '/' + f.filename
#
#                # lê o numero de objetos do dataset
#                with open(datasetlocation) as f:
#                    numObj = sum(1 for _ in f)-1
#
#                newdirectory = False
#                if not os.path.exists(app.config['UPLOADED_PATH'] + '/algResult/'):
#                    os.makedirs(app.config['UPLOADED_PATH'] + '/algResult/')
#                    newdirectory = True
#
#                if(newdirectory):
#                    os.makedirs(app.config['UPLOADED_PATH'] + '/algResult/' + 'results-1/')
#                    # pasta para adicionar resultados tanto do clustering quanto do mocle
#                    resultfolder = app.config['UPLOADED_PATH'] + '/algResult/' + 'results-1/'
#                else:
#                    # pega nome das pastas e ordena, pega a ultima pasta e adiciona mais um para criar a nova
#                    root, dirs, files = next(os.walk(app.config['UPLOADED_PATH'] + '/algResult/'))
#                    # natsorted serve para ordenar 10+
#                    newdir = int((natsorted(dirs)[-1]).split("-")[1]) + 1
#                    os.makedirs(app.config['UPLOADED_PATH'] + '/algResult/' + 'results-' + str(newdir))
#                    # pasta para adicionar resultados tanto do clustering quanto do mocle
#                    resultfolder = app.config['UPLOADED_PATH'] + '/algResult/' + 'results-' + str(newdir)
#
#                minK = int(request.form['minK'])
#                maxK = int(request.form['maxK'])
#
#                # chamar algoritmos
#                if(request.form['basicSelected'] == 'yes'):
#                    # crossover E ou P
#                    tipoDistBasic = request.form['tipoDistBasic']
#                    # algorithms (1, 2, 3, 4, 5, 6, 7)
#                    # se composto, vem com virgula
#                    alg = request.form['alg'].split(',')
#                    print("alg", alg)
#                    if len(alg)>1:
#                        for algnumber in alg:
#                            # passando parametros para função de clustering
#                            print("clustering")
#                            print("algnumber: ", algnumber)
#                            clustering(tipoDistBasic, numObj, minK, maxK, datasetlocation, resultfolder, int(algnumber))
#                    else:
#                        print("else")
#                        clustering(tipoDistBasic, numObj, minK, maxK, datasetlocation, resultfolder, int(alg[0]))
#                        # comentei esse trecho porque tem algum erro de indentação que impede de processar o dropzone
#
#                    if(request.form['mocleSelected'] == 'yes'):
#                        # crossover
#                        # tratar: se M = 1 ou se B = 2
#
#                        if(request.form['tipoDistMocle'] == 'M'):
#                            crossover = 1
#                        else:
#                            crossover = 2
#                            # number of generations
#                            numGen = int(request.form['numGen'])
#                            # nearest neighbours
#                            nearNeigh = int(request.form['nearNeigh'])
#                            # passando parametros para função de mocle
#                            mocle(crossover, datasetlocation, resultfolder + '/AllParts', resultfolder, datasetlocation)
#
#        #path serve para passar a localização do resultado do loadClusters
#        path = '/ds3c3sc6-E-files/'
#
#        # se arquivos vem do partition
#        if(request.form['name'] == 'partition'):
#
#            #pega data e hora atual
#            id = str(datetime.now().strftime('%d-%m-%Y/%H:%M:%S'))
#
#            dir = '/uploaded-part/' + id
#
#            # cria diretorio uploaded na pasta atual, se já não existir
#            if not os.path.exists(app.config['UPLOADED_PATH'] + dir):
#                os.makedirs(app.config['UPLOADED_PATH'] + dir)
#
#            for n in request.files.getlist('datasetfile'):
#                datasetlocation = app.config['UPLOADED_PATH'] + dir
#                n.save(os.path.join(datasetlocation, n.filename))
#                datasetlocation += '/' + n.filename
#
#            if not os.path.exists(app.config['UPLOADED_PATH'] + dir + '/partition'):
#                os.makedirs(app.config['UPLOADED_PATH'] + dir + '/partition')
#
#            # loop over files since we allow multiple files
#            for f in request.files.getlist('file'):
#                #salva arquivos na pasta atual + /uploaded
#                partitionlocation = app.config['UPLOADED_PATH'] + dir + '/partition/'
#                f.save(os.path.join(partitionlocation, f.filename));
#
#            # checa numero de arquivos na pasta para saber se é o ultimo post
#            qtinfolder = len([name for name in os.listdir(app.config['UPLOADED_PATH'] + dir + '/partition/') if os.path.isfile(os.path.join(app.config['UPLOADED_PATH'] + dir + '/partition', name))])
#
#            newdirectory = False
#            if not os.path.exists(app.config['UPLOADED_PATH'] + '/algResult/'):
#                os.makedirs(app.config['UPLOADED_PATH'] + '/algResult/')
#                newdirectory = True
#
#            if(newdirectory):
#                os.makedirs(app.config['UPLOADED_PATH'] + '/algResult/' + 'results-1/')
#                # pasta para adicionar resultados tanto do clustering quanto do mocle
#                resultfolder = app.config['UPLOADED_PATH'] + '/algResult/' + 'results-1/'
#            else:
#                # pega nome das pastas e ordena, pega a ultima pasta e adiciona mais um para criar a nova
#                root, dirs, files = next(os.walk(app.config['UPLOADED_PATH'] + '/algResult/'))
#                newdir = int((natsorted(dirs)[-1]).split("-")[1]) + 1
#                os.makedirs(app.config['UPLOADED_PATH'] + '/algResult/' + 'results-' + str(newdir))
#                # pasta para adicionar resultados tanto do clustering quanto do mocle
#                resultfolder = app.config['UPLOADED_PATH'] + '/algResult/' + 'results-' + str(newdir)
#
#            # compara numero de arquivos na pasta com numero de arquivos aceitos no dropzone
#            if(qtinfolder == int(request.form['qtofdata'])):
#                print("Oi! Você fez upload do partitions!");
#
#                minK = int(request.form['minK'])
#                maxK = int(request.form['maxK'])
#
#                if(request.form['mocleSelected'] == 'yes'):
#                    # tratar: se M = 1 ou se B = 2
#                    if(request.form['tipoDistMocle'] == 'M'):
#                        crossover = 1
#                    else:
#                        crossover = 2
#                    # number of generations
#                    numGen = request.form['numGen']
#                    # nearest neighbours
#                    nearNeigh = request.form['nearNeigh']
#
#                    mocle(crossover, datasetlocation, partitionlocation, resultfolder, datasetlocation)
#                    # TODO: ajustar o resultfolder pra pasta de resultados do
#                    # mocle
#                    # verificar aqui
#
#                # chama loadClusters.py aqui
#                # path = loadCluster(resultfolder)
#
#
#        #path serve para passar a localização do resultado do loadClusters
#        path = '/ds3c3sc6-E-files/'
#
#    return jsonify(path)

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
