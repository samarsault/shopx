#!/usr/bin/env python
# -*- coding: utf-8 -*-

import cgi, cgitb, os, sys
import json
import uuid

UPLOAD_DIR = './upload'

def save_uploaded_file():
	print 'Content-Type: application/json; charset=UTF-8'
	print
	resp = { }
	resp['success'] = False
	form = cgi.FieldStorage()
	if not form.has_key('file'):
		resp['error'] = '<h1>Not found parameter: file</h1>'
		return resp

	form_file = form['file']
	if not form_file.file:
		resp['error'] = '<h1>Not found parameter: file</h1>'
		return resp

	if not form_file.filename:
		resp['error'] = '<h1>Not found parameter: file</h1>'
		return resp

	uploaded_file_path = os.path.join(UPLOAD_DIR, str(uuid.uuid4()) + os.path.splitext(form_file.filename)[1])
	with file(uploaded_file_path, 'wb') as fout:
		while True:
			chunk = form_file.file.read(100000)
			if not chunk:
				break
			fout.write (chunk)

	resp['success'] = True
	resp['url'] = uploaded_file_path[1:]

	return resp

cgitb.enable()
print json.dumps(save_uploaded_file())
