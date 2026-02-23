homedir <- "/home/danielred/data/programs/MCRSbase/OUT/"

toAnalyse <- paste0("runs.1_", 1:6)

library(ggplot2)
library(svglite)


plotgrid2 <- function(fajl, ncol, where, cols, vals, show.values=F, gen=NA) {
  from <- which(fajl == paste0("#", where))
  x <- t(matrix(as.numeric(unlist(strsplit(fajl[(from+1):(from+ncol)], "\t"))), ncol=ncol))
  
  cellcol <- rep("white", ncol*ncol)
  for( c in 1:length(cols) ) {
    cellcol[x==vals[c]] <- cols[c]
  }
  
  color2D.matplot(x,cellcolors=cellcol, show.values=show.values, main=paste("generation:", where), border=NA, axes=F, asp=1, bty="n")
}

#fname = paste0(homedir, "f1/f1.data")
for(azon in toAnalyse) {
  cat("working on ", azon, "\n")
  setwd(paste0(homedir, azon))
  #adat = file(fname, "r")
  adat = file(paste0(azon, ".data"), "r")
  meddig = as.numeric(strsplit(system(paste0("wc -l ", azon, ".data"), intern=TRUE), " ")[[1]][1])-2
  
  #argomentumok beolvas
  argoments <- strsplit(readLines(adat, n=2)[2], " ")
  ncol <- as.numeric(argoments[[1]][1])
  ciklusszam <- as.numeric(argoments[[1]][2])
  met_neigh_meret <- as.numeric(argoments[[1]][3])
  repl_neigh_meret <- as.numeric(argoments[[1]][4])
  phalal <- as.numeric(argoments[[1]][5])
  claimEmpty <- as.numeric(argoments[[1]][6])
  diffuzioGyak <- as.numeric(argoments[[1]][7])
  mintavetel_gyak <- as.numeric(argoments[[1]][8])
  matrixkiiratas_gyak <- as.numeric(argoments[[1]][9])
  modszer <- argoments[[1]][10]
  inicEApar <- as.numeric(argoments[[1]][11])
  inicEA1 <- as.numeric(argoments[[1]][12])
  inicEA2 <- as.numeric(argoments[[1]][13])
  inicEA3 <- as.numeric(argoments[[1]][14])
  kvaluespar <- as.numeric(argoments[[1]][15])
  kvalues1 <- as.numeric(argoments[[1]][16])
  kvalues2 <- as.numeric(argoments[[1]][17])
  kvalues3 <- as.numeric(argoments[[1]][18])
  
  #adatok beolvasasa
  atlagadatok <- data.frame()
  for(sor in 1: min(ceiling(ciklusszam / mintavetel_gyak), meddig) ) {
    atlagadatok = rbind(atlagadatok, as.numeric(strsplit(readLines(adat, n=1), " ")[[1]]))
  }
  colnames(atlagadatok) <- c("generations", "parazite", "empty", "E1", "E2", "E3")
  
  repTime = ggplot(data = atlagadatok, aes(x = generations)) +
    geom_line( aes(y = E1, color="E1") ) +
    geom_line( aes(y = E2, color = "E2") ) +
    geom_line( aes(y = E3, color= "E3") ) +
    geom_line( aes(y = parazite, color="parazite") ) +
    ylab( "number of replicators") +
    scale_color_manual(name = "types", values=c("E1" = "red", "E2" = "green", "E3" = "blue", "parazite" = "black")) +
    labs(title = azon, caption= paste0("ncol= ", ncol, "; met_neigh_meret= ", met_neigh_meret, "; repl_neigh_meret= ", repl_neigh_meret, "; phalal= ", phalal, "; claimEmpty= ", claimEmpty, "; diffuzioGyak= ", diffuzioGyak, "; modszer= ", modszer, "\ninic gyak:", inicEApar, ", ", inicEA1, ", ", inicEA2, ", ", inicEA3, "\nk: ", kvaluespar, ", ", kvalues1, ", ", kvalues2, ", ", kvalues3))
  
  
  ggsave(file= paste0(homedir, azon, "/timeplot.svg"), plot=repTime)
  
  #bezar connection
  close(adat)
  
  #grid kirajzol
  fajl <- readLines(paste0(homedir, azon, "/", azon, "_matrix.data"))
  rmarkdown::render('/home/danielred/data/programs/MCRSbase/src/grid.Rmd',  
                    output_file =  "grids.pdf", 
                    output_dir = paste0(homedir, azon))
}
