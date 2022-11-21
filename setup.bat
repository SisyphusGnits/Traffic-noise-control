rmdir /S /Q iiith_env
py -m pip install --user virtualenv
py -m venv iiith_env
.\iiith_env\Scripts\activate & cd SCRC_DM & pip install -r requirements.txt &python manage.py runserver 0.0.0.0:8000