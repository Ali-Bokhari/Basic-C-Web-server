Ali Bokhari

compile with 'make all'
run server with './server'

Server has the ability to send HTML, CSS, Javascript to the browser as separate files.

If no file specified e.g (http://127.0.0.1:30498) the server defaults to the Example HTML file test.html which links to istyle.css and ijs.js. They are all sent separately with the appropriate content types.

clean up with 'make clean'

max connections wait limit set to 15. (listen functions 2nd argument)
