import requests
import json
import unittest


class TestMicroServer(unittest.TestCase):
  def test_get(self):
    response = requests.get('http://192.168.27.216/')
    print(response.content)

  def test_post_simple(self):
    data = {
      'x': 1.0,
      'y': 2.0,
      'text': 'the quick brown fox jumps over the lazy dog',
      'z': -1.0,
    }

    response = requests.post('http://192.168.27.216/', data=data)
    print(response.content)

  def test_post_complex(self):
    complex_data = {
      'x': 1.0,
      'y': 0.1,
      'theta': 0.0,
      'text': 'hello world'
    }

    response = requests.post('http://192.168.27.216/',
      data={'data': json.dumps(complex_data)})
    print(response.content)


if __name__ == '__main__':
  unittest.main()
