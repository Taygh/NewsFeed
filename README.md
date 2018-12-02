This repository is for the "News Feed" project we did in class. Here is the description:

Our application here is a news feed. The news feed is a collection of current news items produced by our news organization 
(we have reporters, editors, etc. working for us). The news items each contain a headline, a story, and a staleness factor. The headline 
and story are as you would expect. For simplicity, we'll say that the headline is unique and so we can use it as a key into our dictionary 
to retrieve the associated story (in real life we'd have a story ID as the key or maybe a composite key of (headline, author, date) or 
something like that). Each story is rated by an editor based on how interesting it is and how hot the story is. Every once in a while 
(several times per hour), the editor will either upgrade or downgrade the importance of stories as they get hotter or more stale as time 
passes. The editors add stories and change their staleness-factor directly in our application. Meanwhile the reader's application looks at 
the news feed once in a while and pulls off the 5 least stale news items and displays them for the user. We will design our NewsFeed class 
to handle this use case.
