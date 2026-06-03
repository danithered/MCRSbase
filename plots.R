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
  grep("metacentrum", x=_, value=T)|>
  grep("job_info", x=_, value=T, invert=T)

dir("/home/danielred/data/programs/MCRSbase/OUT/") |>
  grep("sim", x=_, value=T) |>
  append(fns) -> fns

# fns <- dir("/home/danielred/data/programs/MCRSbase/OUT/") |>
#   grep("stest8b.1_", x=_, value=T) |>
#   grep("_output_", x=_, value=T, invert=T) 
  
# fn <- "stest8.1_11"
sims <- data.frame()

for(fn in fns)
{
files <- dir(paste0("/home/danielred/data/programs/MCRSbase/OUT/", fn))
files <- files[files != "save"]
url <- paste0("/home/danielred/data/programs/MCRSbase/OUT/", fn, "/", grep("_matrix.data", files, invert=T, value=T))

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
  labs(title = fn, 
       # tag = paste(names(comment), comment, sep="=", collapse="\n") 
       tag = paste(names(comment[c("phalal", "claimEmpty", "modszer", "met_neigh_meret", "repl_neigh_meret")]), comment[c("phalal", "claimEmpty", "modszer", "met_neigh_meret", "repl_neigh_meret")], sep="=", collapse="\n") 
       )+
  theme(plot.tag.location = "panel", plot.tag = element_text(hjust=0)) -> tp
  print(tp)

sims <- rbind(sims, cbind(fn=fn, as.data.frame(comment), df[nrow(df),c("time", "empty", grep("T", names(df), value=T))]))
}

ggplot(sims)+
  geom_point(aes(x=(met_neigh_meret), y=empty, color=(k_4), size=time), position = position_jitter(width=1) )+
  facet_grid(phalal~paste("repl", repl_neigh_meret))+
  scale_size_continuous(range=c(1, 2))
sapply(sims, unique)


str(sims)
apply(sims, 2, unique)


rescaleFact2cont <- function(x){
  as.numeric(droplevels(x))
}

neigh_map <- c("8"="S=25", "16"="S=49", "32"="S=101", "1"="S=5")


sims |>
  filter(repl_neigh_meret==16, phalal==0.0001)|>
  mutate(modszer=factor(modszer, levels=1:11, labels =c("geom mean", "minimum", "harmonic mean", "flat", "random uniform", "Linear flux", "Monod", "geom mean maximized to 1", "minimum maximized to 1", "linear flux maximized to 1", "antifitness maximized to 1"))) |>
  pivot_longer(starts_with("T", ignore.case = F), names_to = "type", values_to = "replicator_count")|>
  ggplot()+
  geom_point(aes(x=modszer, y=replicator_count, color=type))+
  geom_line(aes(x=rescaleFact2cont(modszer), y=replicator_count, color=type))+
  facet_grid(~neigh_map[as.character(met_neigh_meret)])+
  # scale_x_continuous(breaks=1:11, labels = c("geom mean", "minimum", "harmonic mean", "flat", "random uniform", "Linear flux", "Monod", "geom mean maximized to 1", "minimum maximized to 1", "linear flux maximized to 1", "antifitness maximized to 1"))+
  theme(axis.text.x = element_text(angle=45, hjust=1))


sims |>
  filter(repl_neigh_meret==16, phalal==0.0001)|>
  mutate(modszer=factor(modszer, levels=1:11, labels =c("geom mean", "minimum", "harmonic mean", "flat", "random uniform", "Linear flux", "Monod", "geom mean maximized to 1", "minimum maximized to 1", "linear flux maximized to 1", "antifitness maximized to 1"))) |>
  pivot_longer(starts_with("T", ignore.case = F), names_to = "type", values_to = "replicator_count")|>
  group_by(fn) |>
  mutate(rel_freq=replicator_count/sum(replicator_count)) |>
  ggplot()+
  geom_point(aes(x=modszer, y=rel_freq, color=type))+
  geom_line(aes(x=rescaleFact2cont(modszer), y=rel_freq, color=type))+
  facet_grid(~neigh_map[as.character(met_neigh_meret)])+
  # scale_x_continuous(breaks=1:11, labels = c("geom mean", "minimum", "harmonic mean", "flat", "random uniform", "Linear flux", "Monod", "geom mean maximized to 1", "minimum maximized to 1", "linear flux maximized to 1", "antifitness maximized to 1"))+
  theme(axis.text.x = element_text(angle=45, hjust=1))



