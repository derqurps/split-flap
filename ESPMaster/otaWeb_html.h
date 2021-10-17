/*
 * Server upload Page
 */
const char upload_page[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<HTML>
  <HEAD>
    <TITLE>Firmware upload</TITLE>
  </HEAD>
  <BODY>
    <H1>Choose .ino.bin file</H1>
    <form id="uploadform" enctype="multipart/form-data" method="post" action="/update">
       <input id="fileupload" name="inobinfile" type="file" />
       <input type="submit" value="submit" id="submit" />
    </form>
  </BODY>
</HTML>
)=====";
