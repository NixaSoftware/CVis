import os
import sys

activate_this = 'var/www/cvis/venv/bin/activate_this.py'
execfile(activate_this, dict(__file__=activate_this))

sys.stdout = sys.stderr

sys.path.insert(0, os.path.join(os.path.dirname(os.path.realpath(__file__)), '../..'))

sys.path.append('var/www/cvis/')

from main.py import app as application 


