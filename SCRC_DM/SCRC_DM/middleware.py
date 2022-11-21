from django.contrib.sessions.middleware import SessionMiddleware
from django.conf import settings
from django.http import JsonResponse


class NewSessionMiddleware(SessionMiddleware):

    def process_request(self, request):
        session_key = request.COOKIES.get(settings.SESSION_COOKIE_NAME)
        request.session = self.SessionStore(session_key)

    def process_response(self, request, response):
        response_1 = super(NewSessionMiddleware, self).process_response(request, response)
        return response_1


# use custom response class to override HttpResponse.close()
class PostResponseActivity(JsonResponse):
    """
    Class used for performing Post response activities.
    """
    def __init__(self, data, status, safe, req, url):
        headers = {
            "access-control-allow-headers": "*",
            "access-control-allow-methods": "DELETE,POST,GET,OPTIONS,PATCH,PUT",
            "access-control-allow-origin": "*",
        }
        super(PostResponseActivity, self).__init__(data=data, status=status, safe=safe, headers=headers)
        super(PostResponseActivity, self).close()
        self._headers = req
        self._url = url

    def close(self):
        """
        Close the Response and then perform
        @return:
        """
        # Sent the response to user
        pass
        # if self.status_code == 200:
           # print('HttpResponse successful: %s' % self.status_code)


# this would be the view definition
def post_perform(response, status, safe=False, req=None, url=None):
    """

    @param url:
    @param req:
    @param status:
    @param safe:
    @param response:
    @return:
    """
    return PostResponseActivity(response, status, safe=safe, req=req, url=url)


