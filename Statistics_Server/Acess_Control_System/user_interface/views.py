from django.shortcuts import render
from django.http import HttpResponse

# Create your views here.

employees = [
    {
        'card_uid': '1',
        'associated_name': 'Georgi Gostapo',
        'associated_phone_number': '00 359 000 000 000'
    },
    {
        'card_uid': '2',
        'associated_name': 'Ivan Dali Da',
        'associated_phone_number': '00 359 000 000 000'
    }
]


def home(request):
    context = {
        'employees': employees,
        'title': 'Home'
    }
    return render(request, 'user_interface/home.html', context)


def add_user(request):
    context = {
        'title': 'Add User'
    }
    return render(request, 'user_interface/add_user.html', context)


def details(request):
    context = {
        'title': 'Details'
    }
    return render(request, 'user_interface/details.html', context)
