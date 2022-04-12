from unicodedata import name
from django.urls import path
from . import views

urlpatterns = [
    path('', views.home, name='home-page'),
    path('add-user/', views.add_user, name='add-user-page'),
    path('details/', views.details, name='details-page')
]
