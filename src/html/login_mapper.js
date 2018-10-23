function init()
{
  function displayServiceKey(service, key)
  {
    var displayTarget = document.getElementById("show_keys_target");
    if (displayTarget)
      displayTarget.innerHTML =
          "Service: <strong>" + service + "</strong><br/>" +
          "Key: <strong>" + key + "</strong>";
  }

  function displayError(httpCode, errorMessage)
  {
    var displayTarget = document.getElementById("show_keys_target");
    if (displayTarget)
      displayTarget.innerHTML =
          "make_apikey returned <strong>HTTP " + httpCode + "</strong><br/>" +
          errorMessage;
  }

  function triggerGetKey()
  {
    var uid = "";
    var hiddenUid = document.getElementById("uid");
    if (hiddenUid)
      uid = hiddenUid.value;

    var session = "";
    var hiddenSession = document.getElementById("session");
    if (hiddenSession)
      session = hiddenSession.value;

    var service = "";
    var dropdown = document.getElementById("service_selection");
    if (dropdown && dropdown.selectedOptions && dropdown.selectedOptions.length == 1
        && dropdown.selectedOptions[0].label)
      service = dropdown.selectedOptions[0].label;

    var request = new XMLHttpRequest();
    request.open("POST","/apikeys/make_apikey");
    request.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
    request.addEventListener('load', function(event)
    {
      if (request.status == 200)
        displayServiceKey(service, request.responseText);
      else
        displayError(request.status, request.responseText);
    });
    request.send("uid=" + uid + "&session=" + session + "&service=" + service);
  }

  var button = document.getElementById("get_key_button");
  if (button)
    button.addEventListener("click", triggerGetKey);
}
