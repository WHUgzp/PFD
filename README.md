We provide a C implementation of the PFD countermeasure and a hardware implementation on FPGA. 

We also provide a C implementation of the anti-DPA countermeasure based on RP10 scheme from CHES2010, Coron14 scheme from EUROCRPYTO2014.

The countermeasure is shown to be secure against a t-th order DPA attack, when the number of shares n is such that n>=2t+1.

The countermeasure is implemented for the DES and AES block-ciphers on software, and only third-order countermeasure is implemented on FPGA. For comparison we also implement the Rivain-Prouff countermeasure for AES, and the Carlet et al. countermeasure for DES.

What is implemented

    Software implementation for any chosen order t:
        AES without countermeasure
        DES without countermeasure
        AES with Rivain-Prouff countermeasure and Coron countermeasure
        DES with Coron countermeasure, Carlet et al. countermeasure, with the Parity-Split method for Sbox computation (requiring 10 non-linear multiplications), and the improved method from Roy-Vivek (requiring only 7 non-linear multiplications). The latter is used by default. Update: a further improved method from Coron-Roy-Vivek (CHES 2014) is also implemented (requiring only 4 non-linear multiplications).
        AES with PFD countermeasure.
        DES with PFD countermeasure.
    Hardware implementation for the third order:
        AES with PFD countermeasure.

Notes

    We have not protected the key-schedule. Therefore we assume that the block-cipher initially receives the shares of the subkeys, instead of the shares of the key. Moreover we have not implemented the refresh of the key between executions; therefore the implementation would be secure only in a restricted model in which always the same intermediate variables are probed. To get security in the full model one would need to refresh the subkeys between executions.

    We do not claim that in practice the implementation would be secure against a t-th order attack. Namely the implementation is only provided for illustrative purpose, and timing comparisons. Obtaining a secure implementation would require to carefully examine the assembly code. In particular one should make sure that no two shares of the same variable are stored in the same register.
