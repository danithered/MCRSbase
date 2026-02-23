mybinom <- function(n, k) {
  n=floor(n)
  k=floor(k)
  if (k > n) return(0)
  
  if (k > n - k)  k = n - k
  
  result = 1
  
  for (i in 1:k) {
    result = result * (n - k + i) / i
  }
  
  return(result)
}

N=4; X=3; choose(N+X, X)
N=3; X=4; choose(N+X, X)
N=4; X=3; mybinom(N+X, X)
N=3; X=4; mybinom(N+X, X)


for( N in 1:10){
  for( X in 1:N){
  
    print(choose(N+X, X) == mybinom(N+X, X))
  }
}


getIndex <- function(eas){
  noEA = length(eas)
  
  index = 0
  cumsum = 0
  
  for (i in 0:(noEA - 2)) {
    cumsum = cumsum + eas[i + 1]
    p = cumsum + i
    index = index + mybinom(p, i + 1)
  }
  
  return (index)
}

getNo <- function(sumea, noEA){
  if(sumea == 0) return (1)
  if(noEA == 1) return(1)
  return(mybinom(sumea + noEA - 1, noEA - 1))
}

getIndex <- function(eas){
  noEA = length(eas)
  
  sumea = sum(eas)
  if(sumea == 0) return (0) 
  

  index = 0
  
  # forward by sum number of acts
  if(sumea > 0){
    for( i in 0:(sumea - 1)){
      index = index + getNo(i, noEA) 
    }
  }
  
  # calculate index based on position of individual acts
  ea_left = sumea
  pos_left = noEA
  for(ea in eas){
    pos_left = pos_left - 1
    # add positions of eas before
    if(ea>0){
      if(pos_left != 0){
        for(n in 0:(ea-1)){
          index = index + getNo(ea_left-n, pos_left) 
        }
      } 
      ea_left = ea_left - ea
      if(ea_left == 0) return(index)
    }
  }
  return(index)
}


getIndex(c(0,0,0)) #0
getIndex(c(0,0,1)) #1
getIndex(c(0,1,0)) #2
getIndex(c(1,0,0)) #3

getIndex(c(0,0,2)) #4
getIndex(c(0,1,1)) #5
getIndex(c(0,2,0)) #6
getIndex(c(1,0,1)) #7
getIndex(c(1,1,0)) #8
getIndex(c(2,0,0)) #9

generate <- function(pos, remaining, eas, X) {
  if (pos == X - 1) {
    eas[pos+1] = remaining
    print(paste(eas, collapse = ",") )
    return()
  }
  
  for (v in 0:remaining) {
    eas[pos+1] = v
    generate(pos + 1, remaining - v, eas, X)
  }
}

generate_all_compositions <- function(N, X) {
    eas = rep(0, X)
    generate(0, N, eas, X)
}

generate_all_compositions(0, 3)
generate_all_compositions(1, 3)
generate_all_compositions(2, 3)
generate_all_compositions(3, 3)
generate_all_compositions(4, 3)
