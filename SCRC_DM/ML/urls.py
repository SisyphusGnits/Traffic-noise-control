"""
Main URLs file
"""
from django.conf.urls import url
from .views import subscription


urlpatterns = [
    # 127.0.0.1:8000/
	 
    url('create/(?P<node_id>[\w\-]+)$', subscription),
	url('(?P<node_id>(\S)*)$', subscription)
]


