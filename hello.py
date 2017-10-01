from flask import Flask, render_template, request
from flask_dropzone import Dropzone
import os
app = Flask(__name__)
dropzone = Dropzone(app)

app.config['DROPZONE_UPLOAD_MULTIPLE'] = True
app.config['DROPZONE_ALLOWED_FILE_CUSTOM'] = True
app.config['DROPZONE_ALLOWED_FILE_TYPE'] = '.txt, .clu'

# get the current folder
app.config['UPLOADED_PATH'] = os.getcwd()

@app.route('/')
def hello_world():
   return render_template('index.html')

@app.route('/', methods=['GET', 'POST'])
def upload():
    if request.method == 'POST':
        data = request.data
         # loop over files since we allow multiple files
        for f in request.files.getlist('file'):
            print(request.files)
            f.save(os.path.join(app.config['UPLOADED_PATH'], f.filename))
        # chamar algoritmos aqui
    return render_template('index.html')

if __name__ == '__main__':
    app.run(debug=True)
