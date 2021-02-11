
const char net[] PROGMEM = "<!DOCTYPE html><html><head><title>Network Settings</title>\
<script language='JavaScript'> function changeConfirm(f){\
if(!confirm('Are you sure you want to change the Network settings?'))return;f.submit();}\
function getData(){var xmlHttp=new XMLHttpRequest();\
if(xmlHttp==null){return 0;}\
xmlHttp.open('GET','/form_dat',true);\
xmlHttp.responseType = 'text';\
xmlHttp.send(null);\
xmlHttp.onload = function(){\
if(xmlHttp.status==200){var str=xmlHttp.response;\
if(str==null){return 0;}\
else{var i=0;\ 
for(i=0;i<str.length;i++){if(str[i] == ','){\
document.getElementById('ip').value=str.slice(0,i);break;}}\
i=i+1;j=i;\
for(;i<str.length;i++){if(str[i] == ','){\
document.getElementById('msk').value=str.slice(j,i);break;}}\
i=i+1;j=i;\
for(;i<str.length;i++){if(str[i] == ','){\
document.getElementById('gw').value=str.slice(j,i);break;}}\
i=i+1;j=i;\
for(;i<str.length;i++){if(str[i] == ','){\
document.getElementById('mac').value=str.slice(j,i);break;}}\
i=i+1;j=i;\
for(;i<str.length;i++){if(str[i] == ','){\
document.getElementById('ver').value=str.slice(j,i);break;}}\
}}}}\
</script>\
</head><body onload='getData()'><div align=center style='width: 833; height: 470'>\
<table style='border: 1px solid #000080' height=384 cellSpacing=0 cellPadding=0 width=815>\
<tbody><tr style='background-color: green'><td style='border-bottom: 1px solid #000080'></td></tr>\
<tr><td colSpan=5 height=380 width=815 vAlign=top style='background-repeat: repeat-y;'>\
<div align=center><font face='verdana'><table width=90% border=0 align=center><tbody><tr><td width=95%>\
<h2 align=center><br>Network Settings</h2>\
<form action='/network_form' method=get id='my-form'>\
<table border=0 width=70% style='font-size: 14pt' align=center>\
<tr bgcolor=#aaccff><th width=40%>Item</th>\
<th width=60%>Setting</th></tr><tr><td>LAN IP Address</td><td><input type=text name=ip_set id=ip size=18 maxlength=18></td></tr>\
<tr><td>LAN Net Mask</td><td><input type=text name=msk id=msk size=18 maxlength=18></td></tr>\
<tr><td>Default Gateway</td><td><input type=text name=gw id=gw size=18 maxlength=18></td></tr>\
<tr><td>MAC Address</td><td><input type=text name=mac id=mac size=18 maxlength=18></td></tr>\
<tr><td>Version</td>\
<td><input type='text' name=ver id=ver size=18 maxlength=18 disabled=disabled></td></tr>\
</table><p align=center><input type='button' name=set value='Change' onclick='changeConfirm(this.form)'>\
</p></form>\
<tr><td align=center><font face='Verdana'><b> [ <a href='/index.htm'>Return</a> ]</b></font></td></tr>\
</td></tr></tbody></table></font></div></td></tr></tbody></table></div></body>\
</html>";
