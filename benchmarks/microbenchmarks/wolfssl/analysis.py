import re
import sys
filename = str(sys.argv[1])
with open(filename, 'r') as file:
    original_output = file.read()

lines = original_output.split('\n')
pattern = re.compile(r'([A-Za-z0-9-]+)\s+\d+\s+MiB took\s+\d+\.\d+\s+seconds,\s+(\d+\.\d+) MiB/s')
rsa_pattern = re.compile(r'RSA\s+\d+\s+(public|private)\s+\d+\s+ops took\s+\d+\.\d+\s+sec,\s+avg\s+\d+\.\d+\s+ms,\s+(\d+\.\d+) ops/sec')
dh_pattern = re.compile(r'DH\s+\d+\s+(key gen|agree)\s+\d+\s+ops took\s+\d+\.\d+\s+sec,\s+avg\s+\d+\.\d+\s+ms,\s+(\d+\.\d+) ops/sec')
ecc_pattern = re.compile(r'ECC\s+\[\s+SECP256R1\]\s+\d+\s+key gen\s+\d+\s+ops took\s+\d+\.\d+\s+sec,\s+avg\s+\d+\.\d+\s+ms,\s+(\d+\.\d+) ops/sec')
ecdhe_pattern = re.compile(r'ECDHE\s+\[\s+SECP256R1\]\s+\d+\s+agree\s+\d+\s+ops took\s+\d+\.\d+\s+sec,\s+avg\s+\d+\.\d+\s+ms,\s+(\d+\.\d+) ops/sec')
ecdsA_pattern = re.compile(r'ECDSA\s+\[\s+SECP256R1\]\s+\d+\s+(sign|verify)\s+\d+\s+ops took\s+\d+\.\d+\s+sec,\s+avg\s+\d+\.\d+\s+ms,\s+(\d+\.\d+) ops/sec')
pbkdf2_pattern = re.compile(r'PBKDF2\s+\d+\s+KiB took\s+\d+\.\d+\s+seconds,\s+(\d+\.\d+) KiB/s')
gmac_default_pattern = re.compile(r'GMAC Default\s+\d+\s+MiB took\s+\d+\.\d+\s+seconds,\s+(\d+\.\d+) MiB/s')
aes_gcm_no_aad_pattern = re.compile(r'AES-(\d{3})-GCM-enc-no_AAD\s+\d+\s+MiB took\s+\d+\.\d+\s+seconds,\s+(\d+\.\d+) MiB/s')
aes_gcm_dec_no_aad_pattern = re.compile(r'AES-(\d{3})-GCM-dec-no_AAD\s+\d+\s+MiB took\s+\d+\.\d+\s+seconds,\s+(\d+\.\d+) MiB/s')

output_lines = []
for line in lines:
    match = pattern.match(line)
    if match:
        output_lines.append(f'{match.group(1)}:{match.group(2)}')
    else:
        rsa_match = rsa_pattern.match(line)
        if rsa_match:
            output_lines.append(f'RSA 2048 {rsa_match.group(1)}:{rsa_match.group(2)}')
        else:
            dh_match = dh_pattern.match(line)
            if dh_match:
                output_lines.append(f'DH 2048 {dh_match.group(1)}:{dh_match.group(2)}')
            else:
                ecc_match = ecc_pattern.match(line)
                if ecc_match:
                    output_lines.append(f'ECC [SECP256R1] 256 key gen:{ecc_match.group(1)}')
                else:
                    ecdhe_match = ecdhe_pattern.match(line)
                    if ecdhe_match:
                        output_lines.append(f'ECDHE [SECP256R1] 256 agree:{ecdhe_match.group(1)}')
                    else:
                        ecdsA_match = ecdsA_pattern.match(line)
                        if ecdsA_match:
                            output_lines.append(f'ECDSA [SECP256R1] 256 {ecdsA_match.group(1)}:{ecdsA_match.group(2)}')
                        else:
                            pbkdf2_match = pbkdf2_pattern.match(line)
                            if pbkdf2_match:
                                output_lines.append(f'PBKDF2:{pbkdf2_match.group(1)}')
                            else:
                                gmac_default_match = gmac_default_pattern.match(line)
                                if gmac_default_match:
                                    output_lines.append(f'GMAC Default:{gmac_default_match.group(1)}')
                                else:
                                    aes_gcm_no_aad_enc_match = aes_gcm_no_aad_pattern.match(line)
                                    if aes_gcm_no_aad_enc_match:
                                        output_lines.append(f'AES-{aes_gcm_no_aad_enc_match.group(1)}-GCM-enc-no_AAD:{aes_gcm_no_aad_enc_match.group(2)}')
                                    else:
                                        aes_gcm_dec_no_aad_match = aes_gcm_dec_no_aad_pattern.match(line)
                                        if aes_gcm_dec_no_aad_match:
                                            output_lines.append(f'AES-{aes_gcm_dec_no_aad_match.group(1)}-GCM-dec-no_AAD:{aes_gcm_dec_no_aad_match.group(2)}')

new_output = '\n'.join(output_lines)
print(new_output)