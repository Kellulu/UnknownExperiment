alert("Aloha!");  

function FirebaseInit(){
    // Initialize Firebase
    // TODO: Replace with your project's customized code snippet

    var config = {
        apiKey: "<API_KEY>",
        authDomain: "<PROJECT_ID>.firebaseapp.com",
        databaseURL: "https://<DATABASE_NAME>.firebaseio.com",
        storageBucket: "<BUCKET>.appspot.com",
        messagingSenderId: "<SENDER_ID>",
      };
      firebase.initializeApp(config);
    
      // Get a reference to the database service
      var database = firebase.database();
    
      // update the variable when the starCount is changed in the database
      var starCountRef = database.ref('posts/' + postId + '/starCount');
      starCountRef.on('value', function(snapshot) {
        updateStarCount(postElement, snapshot.val());
      });
    
      // update the UI
      function updateStarCount(el, val) {
        el.innerHtml(`${val} Stars!`);
        }
}
