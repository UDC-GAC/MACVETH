```
 File              : README.md
 Author            : Marcos Horro <marcos.horro@udc.gal>
 Date              : Mar 12 Nov 2019 21:51:21 MST
 Last Modified Date: Mér 13 Nov 2019 22:07:44 MST
 Last Modified By  : Marcos Horro <marcos.horro@udc.gal>
```

# $\mu$bench
---

Machine characterization is a tedious task that can require a lot of profiling
by executing many programs or kernels and analyzing their performance. This
tool has been created to automate this task.

This tool is meant for:

* Compiling kernels with different configurations, regarding: loop stride,
  datasizes, etc.
* Execute and time those kernels. There is also the possibility of measuring
  hardware counters using PAPI/C interface.
* Create a CSV/pandas (python) DataFrame with all the configurations and results
  obtained in the execution.


