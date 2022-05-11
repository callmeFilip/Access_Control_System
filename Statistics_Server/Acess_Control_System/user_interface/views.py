from audioop import reverse
from django.http import HttpResponseRedirect
from django.shortcuts import redirect, render
from django.views.generic import ListView, CreateView, UpdateView, DeleteView
from django.contrib import messages
from matplotlib.style import context
from requests import request
from .models import Card, Check_attempt
from .forms import DatesForm
from django.urls import reverse
from django.db.models.query import EmptyQuerySet
import math
import re

"""
def home(request):
    context = {
        'title': 'Home',
        'cards': Card.objects.all(),
    }
    return render(request, 'user_interface/home.html', context)
"""


class EmployeeListView(ListView):
    model = Card
    template_name = 'user_interface/home.html'
    context_object_name = 'cards'
    ordering = ['associated_name']


def EmployeeDetailView(request, pk):    
    context = {
        'title': 'Details',
        'object': Card.objects.filter(card_uid=pk).first()
    }

    if request.method == "POST":
        date_form = DatesForm(request.POST)

        if date_form.is_valid():
            start_date = date_form.cleaned_data['start_date']
            end_date = date_form.cleaned_data['end_date']
        
            # filter all check_attempts based on start_date and end_date for user with pk as id
            check_attempts = Check_attempt.objects.filter(
                card=Card.objects.filter(card_uid=pk).first().card_uid,
                time__gt=start_date, time__lt=end_date)

            # then sort descending by name and pass to context
            context['check_attempts'] = reversed(check_attempts)

            # get only the successful checks in a list
            successful_attempts = check_attempts.filter(status_code=1) # 1 - success


            # calculate worktime
            if len(successful_attempts) >= 2:
                work_time = (successful_attempts[1].time - successful_attempts[0].time)
                if (len(successful_attempts) % 2) is True:
                    for i in range(2, len(successful_attempts), 2):
                        work_time += (successful_attempts[i+1].time - successful_attempts[i].time)
                else:
                    for i in range(2, len(successful_attempts) - 1, 2):
                        work_time += (successful_attempts[i+1].time - successful_attempts[i].time)

                context['work_time_hours'] = math.floor(work_time.total_seconds() / 3600)
                work_time_minutes = math.floor((work_time.total_seconds() / 60) - (context['work_time_hours'] * 60))

                # format minutes
                if work_time_minutes < 10:
                    context['work_time_minutes'] = '0' + str(work_time_minutes)    
                else:
                    context['work_time_minutes'] = work_time_minutes
    else:
        date_form = DatesForm()
    
    context['date_form'] = date_form

    return render(request, "user_interface/employee.html", context)


"""
class EmployeeDetailView(FormMixin, DetailView):
    model = Card
    template_name = 'user_interface/employee.html'
    form_class = DatesForm
   # def setup(request, *args, **kwargs):
   #     print(kwargs['pk'])
   #     return super().setup(request, args, kwargs)
        
    def get_success_url(self):
        return reverse('employee-details-page', kwargs={'pk': self.object.card_uid})

    def get_context_data(self, **kwargs):
        context = super(EmployeeDetailView, self).get_context_data(**kwargs)

        context['title'] = 'Details'
        context['date_form'] = DatesForm(initial={'post': self.object})

        print(self.request.method)
        for item in self.request.POST.lists():
            print(item)

        for key, value in self.request.POST.items():
            print('Key: %s' % (key))
            print('Value %s' % (value))

        start_date = None
        end_date = None

        print(start_date, end_date)

        if start_date and end_date:
            # Get all check_attempts for card with id in 'pk' URL attribute
            context['check_attempts'] = Check_attempt.objects.filter(
                card=Card.objects.filter(card_uid=self.kwargs['pk']).first().card_uid, time__gt=start_date, time__lt=end_date)
            print('1')
        else:
            context['check_attempts'] = Check_attempt.objects.filter(
                card=Card.objects.filter(card_uid=self.kwargs['pk']).first().card_uid)
            print('2')
        return context

    def post(self, request, *args, **kwargs):
        self.object = self.get_object()
        form = self.get_form()
        if form.is_valid():
            print('tuk')
            return self.form_valid(form)
        else:
            return self.form_invalid(form)

    def form_valid(self, form):
        self.start_date = form.cleaned_data['start_date']
        self.end_date = form.cleaned_data['end_date']

        print(self.start_date)
        print(self.end_date)
        self.check_attempts = Check_attempt.objects.filter(
            time__gt=self.start_date, time__lt=self.end_date)
        return super(EmployeeDetailView, self).form_valid(form)

    def form_invalid(self, form):
        messages.error(self.request, 'Invalid Date!')
        return super(EmployeeDetailView, self).form_invalid(form)
"""


class EmployeeCreateView(CreateView):
    model = Card
    fields = ['card_code', 'associated_name',
              'associated_phone_number', 'granted_access_level']

    def form_valid(self, form):  
        temp_card_code = form.data['card_code']
        temp_card_associated_name = form.data['associated_name']
        temp_card_associated_phone_number = form.data['associated_phone_number']
        temp_card_granted_access_level = int(form.data['granted_access_level'])

        # Validate input

        if not (re.match(r'^[A-Z][a-z]{1,34}(\s[A-Z][a-z]{1,34})?(\s[A-Z][a-z]{1,34})?$', temp_card_associated_name)):
            messages.error(self.request, 'Invalid name')
            return super().form_invalid(form)

        if not (re.match(r'^\s*(?:\+?(\d{1,3}))?([-. (]*(\d{3})[-. )]*)?((\d{3})[-. ]*(\d{2,4})(?:[-.x ]*(\d+))?)\s*$', temp_card_associated_phone_number)):
            messages.error(self.request, 'Invalid phone number. Use phone format: +000 000 000 000 or equivalent')
            return super().form_invalid(form)

        if(temp_card_granted_access_level < 0 or temp_card_granted_access_level > 10):
            messages.error(self.request, 'Invalid access level. Choose a value between 0 and 10!')
            return super().form_invalid(form)

        # Check if card already exists

        if Card.objects.filter(card_code=temp_card_code).exists():
            messages.error(
                self.request, f'Employee with Card ID: {temp_card_code} already exists')
            return super().form_invalid(form)
        else:
            self.object = form.save()
            messages.success(
                self.request, f'Employee { temp_card_associated_name } added!')
            return super().form_valid(form)

    def get_context_data(self, **kwargs):
        context = super(EmployeeCreateView, self).get_context_data(**kwargs)

        context['title'] = 'Add'

        return context


class EmployeeUpdateView(UpdateView):
    model = Card
    fields = ['associated_name',
              'associated_phone_number', 'granted_access_level']

    def form_valid(self, form):
        temp_card_associated_name = form.data['associated_name']
        temp_card_associated_phone_number = form.data['associated_phone_number']
        temp_card_granted_access_level = int(form.data['granted_access_level'])
        
        # Validate input

        if not (re.match(r'^[A-Z][a-z]{1,34}(\s[A-Z][a-z]{1,34})?(\s[A-Z][a-z]{1,34})?$', temp_card_associated_name)):
            messages.error(self.request, 'Invalid name')
            return super().form_invalid(form)

        if not (re.match(r'^\s*(?:\+?(\d{1,3}))?([-. (]*(\d{3})[-. )]*)?((\d{3})[-. ]*(\d{2,4})(?:[-.x ]*(\d+))?)\s*$', temp_card_associated_phone_number)):
            messages.error(self.request, 'Invalid phone number. Use phone format: +000 000 000 000 or equivalent')
            return super().form_invalid(form)

        if(temp_card_granted_access_level < 0 or temp_card_granted_access_level > 10):
            messages.error(self.request, 'Invalid access level. Choose a value between 0 and 10!')
            return super().form_invalid(form)

        messages.success(self.request, f'Employee updated!')
        return super().form_valid(form)

    def get_context_data(self, **kwargs):
        context = super(EmployeeUpdateView, self).get_context_data(**kwargs)

        context['title'] = 'Edit'

        return context


class EmployeeDeleteView(DeleteView):
    model = Card
    success_url = '/'


"""
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
                return redirect(new_card.get_absolute_url())

        else:
            print("Invalid Data")

    else:
        add_user_form = AddUserForm()

    context = {
        'title': 'Add User',
        'add_user_form': add_user_form
    }
    return render(request, 'user_interface/card_form.html', context)
"""


"""
def details(request):
    context = {
        'title': 'Details'
    }
    return render(request, 'user_interface/details.html', context)
"""
