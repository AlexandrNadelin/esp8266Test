const char upload[] PROGMEM = "<!DOCTYPE html><html><head>\
<meta http-equiv='Content-Type' content='text/html; charset=utf-8'/>\
<title>File upload</title>\
<script language='JavaScript'> function fileName(path){\
var name=extractFilename(path);\
document.getElementById('upload_visible').value=name;}\
function extractFilename(path) {\
if (path.substr(0, 2) == 'C:'){\
if (path.substr(3, 8) == 'fakepath')return path.substr(12);}\
return path;}\
</script>\
</head>\
<body><div align=center style='width: 833; height: 470'>\
<table style='border: 1px solid #000080' height=384 cellSpacing=0 cellPadding=0 width=815>\
<tbody><tr style='background-color: green'><td style='border-bottom: 1px solid #000080'></td></tr>\
<tr><td colSpan=5 height=380 width=815 vAlign=top style='background-repeat: repeat-y;'>\
<div align=center><font face='verdana'><table width=90% border=0 align=center><tbody><tr><td width=95%>\
<h2 align=center><br>WEB Server File Upload</h2>\
<form action='/upload_file' method=POST enctype='multipart/form-data' name=fileupload>\
<p><font face=verdana><br>Select the file to update:<br><br></font>\
<input type='file' name=upload id='upload_hidden' \
style='position: absolute;display: block;overflow: hidden;width: 0;height: 0;border: 0;padding: 0;' \
onchange='fileName(this.value)'/>\
<input type=text disabled=disabled id='upload_visible'/>\
<input type=button value='Select' onclick=\"document.getElementById('upload_hidden').click();\"/>\
</p><p align=center><input type=button value='Send' OnClick='this.form.submit()'>\
<input type=button value='Return' style='margin-left:20px' onclick=\"location.href='/index.htm';\"/>\
</p></form>\
</td></tr></tbody></table></font></div></td></tr></tbody></table></div></body>\
</html>";
