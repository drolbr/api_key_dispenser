function init()
{
  function displayMessage(service, key)
  {
    var displayTarget = document.getElementById("show_keys_target");
    if (displayTarget)
      displayTarget.innerHTML =
          "Service: <strong>" + service + "</strong><br/>" +
          key;
  }

  function displayServiceKey(service, key)
  {
    var displayTarget = document.getElementById("show_keys_target");
    if (displayTarget)
      displayTarget.innerHTML =
          "Service: <strong>" + service + "</strong><br/>" +
          "Key: <strong>" + key + "</strong>";
  }

  function displayError(target, httpCode, errorMessage)
  {
    var displayTarget = document.getElementById("show_keys_target");
    if (displayTarget)
      displayTarget.innerHTML =
          target + " returned <strong>HTTP " + httpCode + "</strong><br/>" +
          errorMessage;
  }

  function addServiceToServiceList(service)
  {
    var serviceSelection = document.getElementById("service_selection");
    if (serviceSelection)
    {
      var newOption = document.createElement("option");
      newOption.text = service;
      serviceSelection.add(newOption);
    }
  }

  function collectInput()
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

    return { "uid": uid, "session": session, "service": service };
  }

  function triggerResetKey()
  {
    var userInput = collectInput();

    var request = new XMLHttpRequest();
    request.open("POST","/apikeys/make_servicekey");
    request.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
    request.addEventListener('load', function(event)
    {
      if (request.status == 200)
        displayServiceKey(userInput.service, request.responseText);
      else
        displayError("make_servicekey", request.status, request.responseText);
    });
    request.send("uid=" + userInput.uid + "&session=" + userInput.session + "&service=" + userInput.service);
  }

  function triggerRevokeService()
  {
    var userInput = collectInput();

    var request = new XMLHttpRequest();
    request.open("POST","/apikeys/revoke_servicekey");
    request.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
    request.addEventListener('load', function(event)
    {
      if (request.status == 200)
        displayMessage(userInput.service, request.responseText);
      else
        displayError("revoke_servicekey", request.status, request.responseText);
    });
    request.send("uid=" + userInput.uid + "&session=" + userInput.session + "&service=" + userInput.service);
  }

  function triggerAnnounceService()
  {
    var userInput = collectInput();

    var service = "";
    var dropdown = document.getElementById("new_service_uri");
    if (dropdown)
      service = dropdown.value;

    var request = new XMLHttpRequest();
    request.open("POST","/apikeys/announce_service");
    request.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
    request.addEventListener('load', function(event)
    {
      if (request.status == 201)
      {
        displayServiceKey(service, request.responseText);
        addServiceToServiceList(service);
      }
      else
        displayError("announce_service", request.status, request.responseText);
    });
    request.send("uid=" + userInput.uid + "&session=" + userInput.session + "&service=" + service);
  }

  var button = document.getElementById("reset_service_key");
  if (button)
    button.addEventListener("click", triggerResetKey);

  button = document.getElementById("discontinue_service");
  if (button)
    button.addEventListener("click", triggerRevokeService);

  button = document.getElementById("announce_service");
  if (button)
    button.addEventListener("click", triggerAnnounceService);
}
