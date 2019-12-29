# mleck
Market Living Entities.  Artificial Life Cryptocurrency Meanderings.

# What it is.

Mleck is a hobby project building my own machine language algorithms for predicting cryptocurrency markets.  It isn't, however, just about crypto markets.  The way it is constructed, it should be possible to use it for any type of market based prediction mechanism.

For more detail, go to the why oh why oh why... section of the readme.

# Done

-  2019-12-28 Create a backtesting data-set of poloniex data.  Convert it to a binary form.  Create a class to handle the binary data in the program proper.

-  2019-12-28 Create a Farm object that houses mlecks.  Mleck settings files are now created automatically when the farm is created.


# Doing

- So the logic goes like this.  
   -  There are cur_bin objects.  They are representations of a single snapshot of instruments of currency data at a single time.
   -  There are cur_bin_link objects.  These link cur_bin objects into sequences, so they can determine performances, and also record sequential changes between cur_bin objects.
   -  There are jewel objects.  Jewel objects tank cur_bin_link objects and create algorithms that return values based upon those paramatized inputs.

-  Pass a cur_bin object to the farm.  When analyzed, ensure there is a subsequent cur_bin object to the one passed so that performance can be measured.  If a BIN2 file exists for that cur_bin object, load it.  If it doesn't...
   -  Measure the performance of all instruments, comparing 'last' values of the current cur_bin object to the next.
   -  Create a vector of cur_bin objects that step back from the one passed.  
   -  If it runs to the end of the processing round, clone the last one, so that there are a total of 10 (TEN) cur_bin objects available for processing.  
   -  Create vectors of all instrument tickers for the 10 items.  Export all of this information into a .BIN2 file.

# To Do

-  Populate the Mleck logic

-  ... more to come.


# Why oh why oh why oh why...

Mleck is a meandering project I've been working on for almost two decades as a thought experiment on machine learning and AI.  It originally started in VB, went to VB.Net, Java, C#, and finally I've decided to port it to c++.  It originally started as a pure USD/EUR currency pair analysis tool (I have posted a data-set used for that analysis if you're interested), expanded it to analysze many instruments, and then again paired it back to just the USD/EUR analysis.  I have done it on minute, tick, candle, hourly, and just about any other variation that can be thought of at some time or another.

I've taken an epic view of it, in that every time I re-write the program, I do it from scratch, and re-think the logic process about which I'm going to process the information.  I make no attempt to choose an object naming convention that suits other people; Instead I choose one that suits what I think the idea best reflects the implementation as it is being developed.

In this iteration, I have chosen to focus it on cryptocurrency pairs.  One of the biggest problems I've found with these tools in the past is that they follow trends really well.  Until they don't.  Everyone guesses right when everyone is guessing the same way.  In this iteration, I'm not going to apply the system chronologically.  I'm going to randomly select a snapshot of the market at time X, get the instruments to process, make their decisions and breed successful elements based upon performance from that snapshot.  I will then randomly select another element in time, and perform the same analysis.  In order to do that, I needed to get a data-source that met my needs, so two years ago I started downloading data from poloniex in 30 minute snapshots of all of their cryptocurrency instruments.  The data available is the ticker, order book, history, and 300 & 14400 candle data for every instrument in 30 minute intervals over the intervening years.  There are gaps, but this is not really relevant to my processing technique, but there are stretches of months or more with no gaps.  Obviously this is a large data store running at almost 40GB zipped, so I don't want to just dump it on github.  If you are intested, let me know, and I'm sure we can work something out.  I've stopped the download.  It isn't really relevant to me, only the backtesting data is.

What is going to be dumped here however, is a subset of this data-set.  Namely, the USDT_BTC, BTC_ETH, and USDT_ETH pairs.  I've posted a sample of the raw data, then created a c++ utility that writes both formatted ascii and binary files.  The c++ object for reading the binary files will be the the one that I use to load the elements into memory.

No guarantees.  No warrantees.  No funny business whatseover.  Use it as long as you attribute me.  It is a hobby, and it keeps me out of the pub. 
