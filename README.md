<h1>CW1 - Console Game</h1>

<h2>Development environment</h2>
<p>This program was created between two of my computers, a desktop and a laptop. Both computers run Visual Studio Community 2022 (17.7.4), on Windows 11, using C++14.</p>
<p>This project was heavily integrated with git, as it allowed me to seamlessly continue work from one device on another. The commit history will show many commits over time.</p>

<h2>How does it play?</h2>
<p>Upon the program running, the user is prompted to press a key to start the game.</p>
<p>Once the game has been started, the screen displays a 2D map. Other attributes such as time remaining, enemy health, scores, are shown below the map.</p>
<p>The player sprite is a blue "O", while enemies are all red "O"'s. The green "X" is the exit for the map. In some maps, the enemies could be ignored, but points will not be given if the player has not wiped them all out.</p>
<p>To move the player sprite, use the WASD keys on your keyboard. To attack an enemy, the sprite must be moved directly next to an enemy, to where the user can press the spacebar. If an attack is successful, a "*" is shown on the player.</p>
<p>There are 8 maps for the player to complete, and upon finishing all of them, scores are presented. Each map gives an additional 5 points if the time limit has been reached, and all enemies have been defeated.</p>

<h2>UML design diagram</h2>
<p>Diagram was created using https://app.diagrams.net/</p>
<p>Insert image here</p>

<h2>Exception handling and test cases</h2>
<p>The program has a function that can be called from anywhere. It is a error handling function.</p>
<p>When an error is encountered, such as a file being unable to load, the error function will be called, along with the reason as a string parameter.</p>
<p>This will prevent the main program loop from running, and will exit the program early, displaying the details of an error, if one provided by the error caller.</p>

<table>
  <tr>
	<th>Test scenario</th>
	<th>Successful</th>
  </tr>
  <tr>
	<td>The program launches correctly, and experiences no crashes in intended environments.</td>
	<td>✅</td>
  </tr>
  <tr>
	<td>The program successfully loads files, such as map files and audio files.</td>
	<td>✅</td>
  </tr>
  <tr>
	<td>The program can recognise the absence of required data files, and gives notice to the user.</td>
	<td>✅</td>
  </tr>
  <tr>
	<td>The program works as intended, with the player able to progress through levels and earn scores.</td>
	<td>✅</td>
  </tr>
  <tr>
	<td>The program can interpret between different markers in the map files, and apply enemy object data types.</td>
	<td>✅</td>
  </tr>
  <tr>
	<td>The program can identify between different operating systems, notifying the user that compatibility is not intended.</td>
	<td>✅</td>
  </tr>
  <tr>
	<td>The program can compile properly, using post-build commands to copy over data files while retaining the directory structure.</td>
	<td>✅</td>
  </tr>
</table>

<h2>Additional details</h2>
<p>The program uses resources that are separate from the main file. Such files include the maps and audio files. These are stored in the "/data" directory. It is recommended to not modify these files.</p>
<p>It is important that the game has access to these files, otherwise an error will display, showing that a file is unreadable.</p>
<p>Some extra configuartion was required in my development environment. I cannot guarantee that these compilation configurations will replicate, if you were to compile in another environment.</p>
<p>The brief does state that I should use modern C++ over C primitives, however I found that it was difficult to 100% use modern C++.</p>
<p>Although my program doesn't heavily depend on objects, I believe I have covered all the OOP fundamentals. I have overall used classes, constructors, object functions, inheritance, encapsulation and object pooling.</p>
<p>The submission directory will differ slightly, to completely satisfy the requirements. The GitHub will contain source files and release versions.</p>
<p>I believe I have not used any external libraries, as the ones included are either my own header files, or included with C++</p>
<p>I have attempted to implement some USPs into my game, however I am uncertain on their significance and originality. One is the score changing depending on the success of each level, and whether it was completed on time. Another USP could be the dynamic enemy types, and how some have differing properties to others. Another could be the tracking of game data at the bottom of the console game.</p>

<h2>YouTube video</h2>
<p>Link here</p>

<h2>Git repository</h2>
<p>https://github.com/tfoxcroftply/COMP3016</p> 

<h2>Resources (Creative Commons 0 License)</h2>
<p>https://freesound.org/s/441373/</p>
<p>https://freesound.org/s/503493/</p>
<p>https://freesound.org/s/587384/</p>
<p>https://freesound.org/s/171756/</p>
