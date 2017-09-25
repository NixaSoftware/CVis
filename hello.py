from flask import Flask
from flask import render_template
from flask_dropzone import Dropzone
import os
app = Flask(__name__)

app = Flask(__name__)
dropzone = Dropzone(app)

app.config.update(
    UPLOADED_PATH=os.getcwd() + '../upload',
    DROPZONE_ALLOWED_FILE_TYPE='text',
    DROPZONE_INPUT_NAME='data',
)



@app.route('/')
def hello_world():
   return render_template('index.html')
@app.route('/', methods=['POST', 'GET'])
def upload():
    if request.method == 'POST':
        f = request.files.get('data')
        f.save(os.path.join(app.config['UPLOADED_PATH'], f.filename))
    return render_template('index.html')


if __name__ == '__main__':
    app.run(debug=True)
