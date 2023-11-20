<html>
	<body>
		<h1>CW1 - Console Game</h1>
		
		<h2>Development environment</h2>
		<p>This program was created between two of my computers, a desktop and a laptop. Both computers run Visual Studio Community 2022 (17.7.4), on Windows 11, using C++14.</p>
		<p>This project was heavily integrated with git, as it allowed me to seamlessly continue work from one device on another. The commit history will show many commits over time.</p>
		
		<h2>How does it play?</h2>

		<h2>UML design diagram</h2>
		<p>To insert</p>

		<h2>Exception handling and test cases</h2>
		<p>The program has a function that can be called from anywhere. It is a error handling function.</p>
		<p>When an error is encountered, such as a file being unable to load, the error function will be called, along with the reason as a string parameter.</p>
		<p>This will prevent the main program loop from running, and will exit the program early, displaying the details of an error, if one provided by the error caller.</p>
		
		<h2>Additional details</h2>
		<p>The program uses resources that are separate from the main file. Such files include the maps and audio files.</p>
		<p>It is important that the game has access to these files, otherwise an error will display, showing that a file is unreadable.</p>
		<p>The files will be in a resources folder, which will be in the same directory as the game executable.</p>
		<p>On other computers, the resources will likely not copy over into the correct location. Some extra configuartion was required in my development environment.</p>
		<p>The source file will be configured to run without a dedicated resource folder, meaning the files will be in the same directory as the game.</p>
		<p>The brief does state that I should use modern C++ over C primitives, however I found that it was difficult to 100% use modern C++.</p>
		
		<p>Although my program doesn't heavily depend on objects, I believe I have covered all the OOP fundamentals. I have overall used classes, constructors, object functions, inheritance, encapsulation and object pooling.</p>

		<h2>YouTube video</h2>
		<p>Link here</p>

		<h2>Git repository</h2>
		<p>https://github.com/tfoxcroftply/COMP3016</p> 
		
		<h2>Resources (CC0)</h2>
		<p>https://freesound.org/s/441373/</p>
		<p>https://freesound.org/s/503493/</p>
		<p>https://freesound.org/s/587384/</p>
		<p>https://freesound.org/s/171756/</p>
	</body>
</html>