from flask import Flask, render_template, request, flash, redirect, url_for
from flask_dropzone import Dropzone
from datetime import datetime
import json
import os
from loadClusters import *
from script_fcs import *

app = Flask(__name__)
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
    # arqs = []
    if request.method == 'POST':
        # se arquivos vem do dataset
        if(request.form['name'] == 'dataset'):
            if(request.form['partition'] == 'yes'):
                pass
            else:
                #pega data e hora atual
                id = str(datetime.now().strftime('%Y-%m-%d %H:%M:%S'))

                dir = '/uploaded-data/' + id
                # cria diretorio uploaded na pasta atual, se já não existir
                if not os.path.exists(app.config['UPLOADED_PATH'] + dir):
                    os.makedirs(app.config['UPLOADED_PATH'] + dir)

                for f in request.files.getlist('file'):
                    # print para verificar erros no server
                    print(request.files)
                    #salva arquivos na pasta atual + /uploaded
                    f.save(os.path.join(app.config['UPLOADED_PATH'] + dir, f.filename))

                # lê o numero de objetos do dataset
                with open(app.config['UPLOADED_PATH'] + dir + '/' + f.filename) as f:
                    numObj = sum(1 for _ in f)-1

                minK = int(request.form['minK'])
                maxK = int(request.form['maxK'])

                # chamar algoritmos
                if(request.form['basicSelected'] == 'yes'):
                    # crossover E ou P
                    tipoDistBasic = request.form['tipoDistBasic']
                    # algorithms (1, 2, 3, 4, 5, 6, 7)
                    # se composto, vem com virgula
                    print(request.form['alg'])
                    alg = request.form['alg'].split(',')
                    if len(alg)>1:
                        for algnumber in alg:
                            # chamar clustering com int(algnumber)
                            pass

                if(request.form['mocleSelected'] == 'yes'):
                    # crossover
                    # tratar: se M = 1 ou se B = 2
                    if(request.form['tipoDistMocle'] == 'M'):
                        tipoDistMocle = 1
                    else:
                        tipoDistMocle = 2
                    # number of generations
                    numGen = request.form['numGen']
                    # nearest neighbours
                    nearNeigh = request.form['nearNeigh']

        # se arquivos vem do partition
        if(request.form['name'] == 'partition'):

            #pega data e hora atual
            id = str(datetime.now().strftime('%Y-%m-%d %H:%M:%S'))

            dir = '/uploaded-part/' + id

            # cria diretorio uploaded na pasta atual, se já não existir
            if not os.path.exists(app.config['UPLOADED_PATH'] + dir + '/partition'):
                os.makedirs(app.config['UPLOADED_PATH'] + dir + '/partition')

            if not os.path.exists(app.config['UPLOADED_PATH'] + dir):
                os.makedirs(app.config['UPLOADED_PATH'] + dir)

            for n in request.files.getlist('datasetfile'):
                n.save(os.path.join(app.config['UPLOADED_PATH'] + dir, n.filename));

            # loop over files since we allow multiple files
            for f in request.files.getlist('file'):
                #salva arquivos na pasta atual + /uploaded
                f.save(os.path.join(app.config['UPLOADED_PATH'] + dir + '/partition' , f.filename));

            # checa numero de arquivos na pasta para saber se é o ultimo post
            qtinfolder = len([name for name in os.listdir(app.config['UPLOADED_PATH'] + dir + '/partition') if os.path.isfile(os.path.join(app.config['UPLOADED_PATH'] + dir + '/partition', name))])

            # compara numero de arquivos na pasta com numero de arquivos aceitos no dropzone
            if(qtinfolder == int(request.form['qtofdata'])):
                print("Oi! Você fez upload do partitions!");

                minK = int(request.form['minK'])
                maxK = int(request.form['maxK'])

                if(request.form['mocleSelected'] == 'yes'):
                    # tratar: se M = 1 ou se B = 2
                    tipoDistMocle = request.form['tipoDistMocle']
                    # number of generations
                    numGen = request.form['numGen']
                    # nearest neighbours
                    nearNeigh = request.form['nearNeigh']

                    # chama o mocle aqui
                # chama loadClusters.py aqui

    return render_template('index.html')


if __name__ == '__main__':
    app.run(host='200.133.239.42', port=5000)
