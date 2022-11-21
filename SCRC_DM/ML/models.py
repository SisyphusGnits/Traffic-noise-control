from django.db import models

# Create your models here.
#class DataLake(models.Model):
class trafficdata(models.Model):
    """
    Data Lake For Storing Data of all the data
    """
    node_id = models.CharField(max_length=200)
    timestamp = models.DateTimeField(max_length=200,auto_now=True)
    db_val = models.FloatField(max_length=200)
    light_stat = models.CharField(max_length=200,default='GREEN')
    timeout = models.IntegerField(default=3000)
    timeout_inc = models.IntegerField(default=0)



    def __str__(self):
        name = str(self.node_id) + "--" + str(self.timestamp)
        name = name.upper()
        return name

    class Meta:
        unique_together = (("node_id", "timestamp"),)
        ordering = ["-timestamp"]