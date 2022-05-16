fetch('https://api.thingspeak.com/channels/1737377/feeds.json?api_key=PTRM92XMF914F4SV&results=2').then(function(response) {
  // Successfull fetch return as json
  return response.json();
})
.then(function(data) {
  // Data now contains the json
  console.log(data);
})
.catch(function(error) {
  // A Error occured
  console.log(error);
})
