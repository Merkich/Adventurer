# Adventurer

<div id="header" align="center">
  <img src="https://media.giphy.com/media/l2QE3oVEP88zGMTPa/giphy.gif" width="100"/>
</div>

If you accidentally lost a catchphrase in a document,
then this app will help you find it.


### :writing_hand: What is done in the application  :
This application searches for words and sentences in a set of documents. The result of the application will be a structure in which the files with the most frequently repeated requests will be displayed.

### :technologist: What was used to create the application  : 
The application was developed in the QT framework, using json files and multithreading elements. A graphic designer was also used to visualize the application with a clear interface.

### :woman_scientist: How to use this application:
To run the application, you need to download the project, go to the folder with the application version: search_engine/release/.
Launch application search_engine.exe double click.

In the application window, you must specify the path to the configuration file configure.json (the folder with the configuration file should contain a file with a request, a file with an answer will appear in this folder after the application ends). To do this, click on the "Указать путь" button. Choose a configuration file. 

Click the button "Выполнить запрос" to complete the request.


After the application completes, the answer.json file will appear in the folder specified earlier.

### :warning: 
For the configuration file and the file with requests, you must use the json structure.

After the application finishes its work, a message will be displayed in the  field "Результат выполнения запроса" with the result of processing the request.

If errors were detected while the application was running, they will be displayed in the field "Ошибки выполнения запроса".


### :grey_exclamation: 
If you have changed the application code, you can run tests to verify that it works correctly.
