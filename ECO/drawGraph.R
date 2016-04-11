
args <- commandArgs(trailingOnly = TRUE)
S <- as.numeric(args[1])
landa <- as.numeric(args[2])
divisions <- as.integer(args[3])
rm(args)


# Read data from .dat
samples_data <- read.table("/Users/Capa/College/LAStability/LAStability/LAStability/results.dat", header=T, sep="\t")
# Compute the largest y value used in the data (or we could
# just use range again)
max_y <- max(samples_data)

# Define colors to be used for V(x-), V(x+), D
plot_colors <- c("blue","red","forestgreen","black","purple")

# Start PNG device driver to save output to figure.png
png(filename="/Users/Capa/College/LAStability/LAStability/graph.png", height=500, width=800,
 bg="white")

# Graph autos using y axis that ranges from 0 to max_y.
# Turn off axes and annotations (axis labels) so we can
# specify them ourself
plot(samples_data$Vxn, type="o", col=plot_colors[1],pch=22,lty=2,
   ylim=c(0,max_y), axes=FALSE, ann=FALSE)

# Make x axis
axis(1, las=1,at=0:divisions)

# Make y axis with horizontal labels that display ticks at
# every 4 marks. 4*0:max_y is equivalent to c(0,4,8,12).
axis(2, las=1, at=2*0:max_y)

# Create box around plot
box()

# Graph V(x+) with red dashed line and square points
lines(samples_data$Vxp, type="o", pch=22, lty=2,
   col=plot_colors[2])

# Graph D with green dotted line and diamond points
lines(samples_data$D, type="o", col=plot_colors[3])

# S value, average of sample obtained
abline(h = S, v = 0, col = plot_colors[4])

# Landa value, expected stability threshold
abline(h = landa, v = 0, col = plot_colors[5])


# Create a title with a red, bold/italic font
title(main="LA Analisis", col.main="red", font.main=4)

# Label the x and y axes with dark green text
title(xlab= "Time", col.lab=plot_colors[1])
title(ylab= "Load", col.lab=plot_colors[1])

# Create a legend at (1, max_y) that is slightly smaller
# (cex) and uses the same line colors and points used by
# the actual plots
legend_names <- c(names(samples_data),"S","threshold")

legend(1, max_y, legend_names, cex=0.8, col=plot_colors,
   pch=21:23, lty=1:5);

# Turn off device driver (to flush output to png)
dev.off()
