library(ggplot2)
library(tidyr)
library(dplyr)

lines = readLines("/home/danielred/data/programs/MCRSbase/OUT/f1np/f1np.data")
pars = lines[1:2]
lines = lines[c(-1, -2)]
table <- read.table(text=lines)
colnames(table) = c("generations", "parazite", "empty", "E1", "E2", "E3")


table |>
  select(!empty)|>
  pivot_longer(!generations) |>
  ggplot(aes(x=generations, y=value))+
  geom_area(aes(fill=name))+
  theme_minimal()+
  labs(title="Geometric mean")

ggsave("/home/danielred/data/alma/andrasnak/2026_02_24/geometric_mean2.png", width=10, height=5)


lines = readLines("/home/danielred/data/programs/MCRSbase/OUT/f2np/f2np.data")
pars = lines[1:2]
lines = lines[c(-1, -2)]
table <- read.table(text=lines)
colnames(table) = c("generations", "parazite", "empty", "E1", "E2", "E3")

table |>
  select(!empty)|>
  pivot_longer(!generations) |>
  ggplot(aes(x=generations, y=value))+
  geom_area(aes(fill=name))+
  theme_minimal()+
  labs(title="Minimum")

ggsave("/home/danielred/data/alma/andrasnak/2026_02_24/minimum2.png", width=10, height=5)

lines = readLines("/home/danielred/data/programs/MCRSbase/OUT/f4np/f4np.data")
pars = lines[1:2]
lines = lines[c(-1, -2)]
table <- read.table(text=lines)
colnames(table) = c("generations", "parazite", "empty", "E1", "E2", "E3")

table |>
  select(!empty)|>
  pivot_longer(!generations) |>
  ggplot(aes(x=generations, y=value))+
  geom_area(aes(fill=name))+
  theme_minimal()+
  labs(title="Flat")

ggsave("/home/danielred/data/alma/andrasnak/2026_02_24/flat2.png", width=10, height=5)


lines = readLines("/home/danielred/data/programs/MCRSbase/OUT/t2/t2.data")
pars = lines[1:2]
lines = lines[c(-1, -2)]
table <- read.table(text=lines)
colnames(table) = c("generations", "parazite", "empty", "E1", "E2", "E3")

table |>
  select(!empty)|>
  pivot_longer(!generations) |>
  ggplot(aes(x=generations, y=value))+
  geom_area(aes(fill=name))+
  theme_minimal()+
  labs(title="Random landscape")

ggsave("/home/danielred/data/alma/andrasnak/2026_02_24/random.png", width=10, height=5)




fns <- dir("/home/danielred/data/programs/MCRSbase/OUT/") |>
  grep("stest8b.1_", x=_, value=T) |>
  grep("_output_", x=_, value=T, invert=T) 
  
# fn <- "stest8.1_11"
sims <- data.frame()

for(fn in fns)
{
url <- paste0("/home/danielred/data/programs/MCRSbase/OUT/", fn, "/", fn, ".data")
comment <- grep("#",readLines(url), fixed=T, value=T)
comment |>
  gsub("# ", "", x=_) |>
  strsplit(" ", fixed=T) |>
  unlist() |>
  strsplit("=") -> comment
ns <- lapply(comment, function(x) x[1]) 
comment <- lapply(comment, function(x) {
  ifelse(is.na(as.numeric(x[2])), x[2], as.numeric(x[2]))
}) 
names(comment) <- ns

df <- read.table(url, header=T) 
df |>
  pivot_longer(!time) |>
  ggplot()+
  geom_line(aes(x=time, y=value, color=name))+
  labs(title = fn, tag = paste(names(comment), comment, sep="=", collapse="\n") )+
  theme(plot.tag.location = "panel", plot.tag = element_text(hjust=0)) -> tp
  print(tp)

sims <- rbind(sims, cbind(fn=fn, as.data.frame(comment), df[nrow(df),c("time", "empty")]))
}

ggplot(sims)+
  geom_point(aes(x=(met_neigh_meret), y=empty, color=(k_4), size=time), position = position_jitter(width=1) )+
  facet_grid(phalal~paste("repl", repl_neigh_meret))+
  scale_size_continuous(range=c(1, 2))
sapply(sims, unique)

