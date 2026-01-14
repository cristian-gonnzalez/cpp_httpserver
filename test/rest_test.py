import requests
import json


headers = {"Content-Type":"application/json"}
payload = { 'a' : 1, 'b' : 1 }
url = "http://127.0.0.1:8080/add"

print(url, headers, payload)

response = requests.post(url, data=json.dumps(payload), headers=headers)
print(response.status_code, response.text)