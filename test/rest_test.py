import requests
import json


headers = {"Content-Type":"application/json"}
payload = {}
url = "http://127.0.0.1:8080"


response = requests.post(url, data=json.dumps(payload), headers=headers)
print(response)