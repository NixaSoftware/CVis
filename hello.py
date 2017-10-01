from flask import Flask, render_template, request
from flask_dropzone import Dropzone
import os
app = Flask(__name__)
dropzone = Dropzone(app)

app.config['DROPZONE_UPLOAD_MULTIPLE'] = True

# get the current folder
app.config['UPLOADED_PATH'] = os.getcwd() + '/upload'

@app.route('/')
def hello_world():
   return render_template('index.html')

@app.route('/upload', methods=['GET', 'POST'])
def upload():
    if request.method == 'POST':
         # loop over files since we allow multiple files
        for f in request.files.getlist('file'):
            print(request.files)
            f.save(os.path.join(app.config['UPLOADED_PATH'], f.filename))
        # chamar algoritmos aqui
    return "OK"

if __name__ == '__main__':
    app.run(debug=True)
