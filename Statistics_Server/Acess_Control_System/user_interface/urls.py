from unicodedata import name
from django.urls import path
from . import views

urlpatterns = [
    path('', views.EmployeeListView.as_view(), name='home-page'),
    path('employee/new/', views.EmployeeCreateView.as_view(), name='add-user-page'),
    path('employee/<int:pk>/', views.TestView, name = 'employee-details-page'),
    # path('employee/<int:pk>/', views.EmployeeDetailView.as_view(), name='employee-details-page'),
    path('employee/<int:pk>/update/', views.EmployeeUpdateView.as_view(),
         name='employee-update-page'),
    path('employee/<int:pk>/remove/', views.EmployeeDeleteView.as_view(),
         name='employee-remove-page')
]
