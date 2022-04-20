from django import forms


class AddUserForm(forms.Form):
    card_code = forms.CharField(max_length=40)
    associated_name = forms.CharField(max_length=40)
    associated_phone_number = forms.CharField(max_length=20)
    granted_access_level = forms.IntegerField(min_value=0, max_value=10)


class DatesForm(forms.Form):
    start_date = forms.DateField(widget=forms.DateInput(
        attrs={'placeholder': 'YYYY-MM-DD', 'required': 'required'}))
    end_date = forms.DateField(widget=forms.DateInput(
        attrs={'placeholder': 'YYYY-MM-DD', 'required': 'required'}))
