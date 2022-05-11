from django.shortcuts import render
from django.views.generic import ListView, CreateView, UpdateView, DeleteView
from django.contrib import messages
from requests import request
from .models import Card, Check_attempt
from .forms import DatesForm
from math import floor
from re import match


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

                context['work_time_hours'] = floor(work_time.total_seconds() / 3600)
                work_time_minutes = floor((work_time.total_seconds() / 60) - (context['work_time_hours'] * 60))

                # format minutes
                if work_time_minutes < 10:
                    context['work_time_minutes'] = '0' + str(work_time_minutes)    
                else:
                    context['work_time_minutes'] = work_time_minutes
    else:
        date_form = DatesForm()
    
    context['date_form'] = date_form

    return render(request, "user_interface/employee.html", context)


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

        if not (match(r'^[A-Z][a-z]{1,34}(\s[A-Z][a-z]{1,34})?(\s[A-Z][a-z]{1,34})?$', temp_card_associated_name)):
            messages.error(self.request, 'Invalid name')
            return super().form_invalid(form)

        if not (match(r'^\s*(?:\+?(\d{1,3}))?([-. (]*(\d{3})[-. )]*)?((\d{3})[-. ]*(\d{2,4})(?:[-.x ]*(\d+))?)\s*$', temp_card_associated_phone_number)):
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

        if not (match(r'^[A-Z][a-z]{1,34}(\s[A-Z][a-z]{1,34})?(\s[A-Z][a-z]{1,34})?$', temp_card_associated_name)):
            messages.error(self.request, 'Invalid name')
            return super().form_invalid(form)

        if not (match(r'^\s*(?:\+?(\d{1,3}))?([-. (]*(\d{3})[-. )]*)?((\d{3})[-. ]*(\d{2,4})(?:[-.x ]*(\d+))?)\s*$', temp_card_associated_phone_number)):
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
