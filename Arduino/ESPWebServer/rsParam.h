const char rs[] PROGMEM = "<!DOCTYPE html><html>\
<head><title>COM port parameters</title>\
<script language='JavaScript'>\
function getData(){var xmlHttp=new XMLHttpRequest();\
if(xmlHttp==null){return 0;}\
xmlHttp.open('GET','/form_dat_rs',true);\
xmlHttp.responseType = 'text';\
xmlHttp.send(null);\
xmlHttp.onload = function(){\
if(xmlHttp.status==200){var str=xmlHttp.response;\
if(str==null){return 0;}\
else{var i=0;\ 
for(i=0;i<str.length;i++){if(str[i] == ','){\
document.getElementById('baud').selectedIndex=Number.parseInt(str.slice(0,i));break;}}\
i=i+1;j=i;\
for(;i<str.length;i++){if(str[i] == ','){\
document.getElementById('data').selectedIndex=Number.parseInt(str.slice(j,i));break;}}\
i=i+1;j=i;\
for(;i<str.length;i++){if(str[i] == ','){\
document.getElementById('stop').selectedIndex=Number.parseInt(str.slice(j,i));break;}}\
i=i+1;j=i;\
for(;i<str.length;i++){if(str[i] == ','){\
document.getElementById('parity').selectedIndex=Number.parseInt(str.slice(j,i));break;}}\
i=i+1;j=i;\
for(;i<str.length;i++){if(str[i] == ','){\
document.getElementById('hand').selectedIndex=Number.parseInt(str.slice(j,i));break;}}\
i=i+1;j=i;\
for(;i<str.length;i++){if(str[i] == ','){\
document.getElementById('simvol').selectedIndex=Number.parseInt(str.slice(j,i));break;}}\
i=i+1;j=i;\
for(;i<str.length;i++){if(str[i] == ','){\
document.getElementById('nullSim').selectedIndex=Number.parseInt(str.slice(j,i));break;}}\
}}}}\
</script>\
</head>\
<body onload='getData()'><div align=center style='width: 833; height: 470'>\
<table style='border: 1px solid #000080' height=384 cellSpacing=0 cellPadding=0 width=815>\
<tbody><tr style='background-color: green'><td style='border-bottom: 1px solid #000080'></td></tr>\
<tr><td colSpan=5 height=380 width=815 vAlign=top style='background-repeat: repeat-y;'>\
<div align=center><font face='verdana'><table width=90% border=0 align=center><tbody><tr><td width=95%>\
<h2 align=center><br>Setting COM port parameters</h2>\
<form action='/rs_param' method=post name=cgi>\
<table border=0 width=70% style='font-size: 14pt' align='center'><tr bgcolor=#aaccff>\ 
<th width=40%>Item</th>\
<th width=60%>Setting</th></tr><tr><td>Baud rate:</td>\
<td><select name='Baud' id=baud onchange='submit()'>\
<option>4800</option><option>9600</option><option>19200</option><option>38400</option>\
</select></td></tr><tr><td>Data bits:</td><td><select name='Data' id=data onchange='submit()'>\
<option>7</option><option>8</option></select></td></tr>\
<tr><td>Stop bits:</td><td><select name=Stop id=stop onchange='submit()'>\
<option>1</option><option>2</option></select></td></tr>\
<tr><td>Parity:</td><td><select name=Parity id=parity onchange='submit()'>\
<option>Even</option><option>Odd</option><option>None</option></select></td></tr>\
<tr><td>Handshaking:</td><td><select name=Hand id=hand onchange='submit()'>\
<option>RTS/CTS</option><option>XON/XOFF</option></select></td></tr>\
<tr><td>CR select:</td><td><select name=Simvol id=simvol onchange='submit()'>\
<option>None</option><option>Yes</option></select></td></tr>\
<tr><td>Add empty symbols:</td>\
<td><select name=NullSim id=nullSim onchange='submit()'>\
<option>None</option><option>Yes</option></select></td></tr></table>\
<p align=center><input type=button value='Return' onclick=\"location.href='/index.htm';\"/>\
</p></form>\
</td></tr></tbody></table></font></div></td></tr></tbody></table></div></body>\
</html>";
