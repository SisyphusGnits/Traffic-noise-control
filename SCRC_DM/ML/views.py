"""
Main Views for applying ML Model
"""
from django.views.decorators.csrf import csrf_exempt
from django.http import JsonResponse
import after_response
import json
import time
from .models import trafficdata




def subscription(request, node_id=""):
    """
    Main Subscription view for receiving OM2M Data
    @return:
    """
    if request.method == "OPTIONS":

        headers = {
        "access-control-allow-headers": "*",
        "access-control-allow-methods": "DELETE,POST,GET,OPTIONS,PATCH,PUT",
        "access-control-allow-origin": "*",
        }
        return JsonResponse(data={}, status=status, safe=False, headers=headers)
    try:
        data = json.loads(request.body)
        print(data)
        con = eval(data["m2m:sgn"]["m2m:nev"]["m2m:rep"]["m2m:cin"]["con"])
        apply_ml.after_response(con)
        return JsonResponse({'200': 'OK'})
    except Exception as General_Exception:
        print(General_Exception)
        return JsonResponse({'200': 'OK'})

@after_response.enable
def apply_ml(con):
    """

    @param con:
    """
    time.sleep(5)
    print(con)


    table_obj = trafficdata(node_id=con[0], db_val=con[1], light_stat = con[2], timeout = con[3], timeout_inc=con[4])
    #_handle_fact = Data_Fact(Node_id=_name, Timestamp=response[0], Version=response[2], data=response[1])
    table_obj.save()
    print("saved to database")

    #_handle_fact.save(using=_db)
