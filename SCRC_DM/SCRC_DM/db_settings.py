"""
Holds the Settings related to Databases
"""
from pathlib import Path

BASE_DIR = Path(__file__).resolve().parent.parent
'''
db_config = {
        'ENGINE': 'django.db.backends.postgresql',
        'NAME': 'productionDB',
        'USER': 'admin',
        'PASSWORD': '1234567890',
        'HOST': 'localhost',
        'PORT': '9500',
}
'''

db_config = {
        'ENGINE': 'django.db.backends.postgresql',
        'NAME': 'sisyphus',
        'USER': 'postgres',
        'PASSWORD': 'hyderbad',
        'HOST': 'localhost',
        'PORT': '5432',
}


DATABASES = {
    'default': {
        #'ENGINE': 'django.db.backends.sqlite3',
        #'NAME': BASE_DIR / 'core.sqlite3',
        'ENGINE': 'django.db.backends.postgresql',
        'NAME': 'sisyphus',
        'USER': 'postgres',
        'PASSWORD': 'hyderbad',
        'HOST': 'localhost',
        'PORT': '5432',
    }
}