from django.shortcuts import redirect, render
from django.contrib import messages
from .models import Card, Check_attempt
from .forms import AddUserForm

# Create your views here.


def home(request):
    context = {
        'title': 'Home',
        'cards': Card.objects.all(),
        'check_attempts': Check_attempt.objects.all()
    }
    return render(request, 'user_interface/home.html', context)


def add_user(request):

    if request.method == 'POST':
        add_user_form = AddUserForm(request.POST)

        if add_user_form.is_valid():
            n_card_code = add_user_form.cleaned_data['card_code']
            n_associated_name = add_user_form.cleaned_data['associated_name']
            n_associated_phone_number = add_user_form.cleaned_data['associated_phone_number']
            n_granted_access_level = add_user_form.cleaned_data['granted_access_level']
            print(n_card_code, n_associated_name,
                  n_associated_phone_number, n_granted_access_level)

            if Card.objects.filter(card_code=n_card_code).exists():
                messages.error(
                    request, f'Employee with Card ID: {n_card_code} already exists')
            else:
                new_card = Card(card_code=n_card_code, associated_name=n_associated_name,
                                associated_phone_number=n_associated_phone_number, granted_access_level=n_granted_access_level)
                new_card.save()
                messages.success(
                    request, f'Employee {n_associated_name} added!')
                return redirect('home-page')

        else:
            print("Invalid Data")

    else:
        add_user_form = AddUserForm()

    context = {
        'title': 'Add User',
        'add_user_form': add_user_form
    }
    return render(request, 'user_interface/add_user.html', context)


def details(request):
    context = {
        'title': 'Details'
    }
    return render(request, 'user_interface/details.html', context)
