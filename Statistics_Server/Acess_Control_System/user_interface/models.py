from django.db import models
from django.utils import timezone
from django.urls import reverse

# Create your models here.


class Card(models.Model):
    card_uid = models.AutoField(primary_key=True)
    associated_name = models.CharField(max_length=40)
    associated_phone_number = models.CharField(max_length=20)
    granted_access_level = models.IntegerField()
    card_code = models.CharField(max_length=40)

    def __str__(self):
        return str(self.card_code)

    def get_absolute_url(self):
        return reverse('employee-details-page', kwargs={'pk': self.pk})

    class Meta:
        db_table = 'card'


class Check_attempt(models.Model):
    attempt_id = models.AutoField(primary_key=True)
    card = models.ForeignKey(Card, db_column='card',
                             on_delete=models.SET('deleted'))
    device_name = models.CharField(max_length=40)
    status_code = models.IntegerField()
    time = models.DateTimeField(default=timezone.now)

    def __str__(self):
        return str(self.attempt_id)

    class Meta:
        db_table = 'check_attempt'
