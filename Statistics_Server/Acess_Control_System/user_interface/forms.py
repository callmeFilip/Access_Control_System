from django import forms
from numpy import unsignedinteger


class AddUserForm(forms.Form):
    card_code = forms.CharField(max_length=40)
    associated_name = forms.CharField(max_length=40)
    associated_phone_number = forms.CharField(max_length=20)
    granted_access_level = forms.IntegerField(min_value=0, max_value=10)
