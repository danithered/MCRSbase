library(plotrix)

fajl <- readLines("/home/danielred/data/programs/MCRSbase/OUT/testrun/testrun_matrix.data")
ncol <- 6


number <- 0
where <- 1

plotgrid <- function(fajl, ncol, numbers, where) {
  par(mfrow=c(length(numbers), 1))
  for(number in numbers){
    k <- c(where-ncol, where-1, where+1, where+ncol)
    k = (( ncol^2 -((ncol^2-k) %% ncol^2))) %% ncol^2
    
    jel <- rep(0, ncol*ncol)
    jel[where+1] <- 1
    jel[k+1] <- 2
    m1 <- matrix(jel, ncol=ncol, byrow=F)
    color <- which(t(m1) == 1)
    neigh <- which(t(m1) == 2)
  
    from <- grep(paste0("#", number), fajl)
    x <- t(matrix(as.numeric(unlist(strsplit(fajl[(from+1):(from+ncol)], "\t"))), ncol=ncol))
    cellcol <- rep("white", ncol*ncol)
    cellcol[color] <- "red"
    cellcol[neigh] <- "lightblue"
  
    color2D.matplot(x,cellcolors=cellcol, show.values=T, main=paste("generation", number, ", position:", where))
  }
  par(mfrow=(c(1,1)))
}

plotgrid(fajl, ncol, 0:1, 12)

x <- t(matrix(as.numeric(unlist(strsplit(fajl[(from+1):(from+ncol)], "\t"))), ncol=ncol))
table(x)

0 8 7 6 9 6
1 8 7 6 8 7
2 8 10 6 7 5
3 7 16 5 5 3
4 6 18 5 4 3
5 6 20 3 4 3


