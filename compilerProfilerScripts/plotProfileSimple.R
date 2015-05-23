#!/usr/bin/Rscript
args=commandArgs(TRUE)

library(ggplot2)
library(grid)

multiplot <- function(plotList, rows, cols, layout=NULL) {
  
  numPlots = length(plotList)
  plotsPerPage = rows * cols;
  numPages = ceiling(numPlots/plotsPerPage);
  
  layout <- matrix(seq(1, plotsPerPage),
                   ncol = cols, nrow = rows)
  if (numPlots==1) {
    print(plots[[1]])
    
  } else {
    for(page in 1:numPages){
      # Set up the page
      grid.newpage()
      pushViewport(viewport(layout = grid.layout(nrow(layout), ncol(layout))))
      
      # Make each plot, in the correct location
      for (i in 1:plotsPerPage) {
        # Get the i,j matrix positions of the regions that contain this subplot
        matchidx <- as.data.frame(which(layout == i, arr.ind = TRUE))
        if((i + (page - 1) * plotsPerPage) > length(plotList)){
          
        }else{
          print(plotList[[i + (page - 1) * plotsPerPage]], vp = viewport(layout.pos.row = matchidx$row,
                                                                         layout.pos.col = matchidx$col))
        }
      }
    }
  }
}

tim = read.delim(args[1])
pdf(args[2], width = 11, height = 8.5)
#multiplot(list(ggplot(aggregate(tim$time.sec., tim[,1:7], mean), aes(x = numType, y = x, fill = compiler) ) + geom_bar(stat = "identity", position = "dodge") +ylab("time(sec)"), ggplot(aggregate(tim$time.sec., tim[,1:7], mean), aes(x = compiler, y = x, fill = numType) ) + geom_bar(stat = "identity", position = "dodge") +ylab("time(sec)")) ,2, 1)
ggplot(aggregate(tim$time.sec., tim[,1:7], mean), aes(x = numType, y = x, fill = compiler) ) + geom_bar(stat = "identity", position = "dodge") +ylab("time(sec)") + facet_grid(~alnType, as.table = T)
ggplot(aggregate(tim$time.sec., tim[,1:7], mean), aes(x = compiler, y = x, fill = numType) ) + geom_bar(stat = "identity", position = "dodge") +ylab("time(sec)") + facet_grid(~alnType, as.table = T)
dev.off()