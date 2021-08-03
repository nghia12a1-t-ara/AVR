long IRrecv::decodeNEC(decode_results *results) {
  long data = 0;
  int offset = 1; // Skip first space
  // Initial mark
  if (!MATCH_MARK(results->rawbuf[offset], NEC_HDR_MARK)) {
    return ERR;
  }
  offset++;
  // Check for repeat
  if (irparams.rawlen == 4 &&
    MATCH_SPACE(results->rawbuf[offset], NEC_RPT_SPACE) &&
    MATCH_MARK(results->rawbuf[offset+1], NEC_BIT_MARK)) {
    results->bits = 0;
    results->value = REPEAT;
    results->decode_type = NEC;
    return DECODED;
  }
  if (irparams.rawlen < 2 * NEC_BITS + 4) {
    return ERR;
  }
  // Initial space  
  if (!MATCH_SPACE(results->rawbuf[offset], NEC_HDR_SPACE)) {
    return ERR;
  }
  offset++;
  for (int i = 0; i < NEC_BITS; i++) {
    if (!MATCH_MARK(results->rawbuf[offset], NEC_BIT_MARK)) {
      return ERR;
    }
    offset++;
    if (MATCH_SPACE(results->rawbuf[offset], NEC_ONE_SPACE)) {
      data = (data << 1) | 1;
    } 
    else if (MATCH_SPACE(results->rawbuf[offset], NEC_ZERO_SPACE)) {
      data <<= 1;
    } 
    else {
      return ERR;
    }
    offset++;
  }
  // Success
  results->bits = NEC_BITS;
  results->value = data;
  results->decode_type = NEC;
  return DECODED;
}

int IR_decode(decode_results *results) {
	results->rawbuf = irparams.rawbuf;
	results->rawlen = irparams.rawlen;
	if (irparams.rcvstate != STATE_STOP) {
		return ERR;
	}
	#ifdef DEBUG
	Serial.println("Attempting NEC decode");
	#endif
	if (decodeNEC(results)) {
		return DECODED;
	}
	#ifdef DEBUG
	Serial.println("Attempting Sony decode");
	#endif
	if (decodeSony(results)) {
		return DECODED;
	}
	#ifdef DEBUG
	Serial.println("Attempting Sanyo decode");
	#endif
	if (decodeSanyo(results)) {
		return DECODED;
	}
	#ifdef DEBUG
	Serial.println("Attempting Mitsubishi decode");
	#endif
	if (decodeMitsubishi(results)) {
		return DECODED;
	}
	#ifdef DEBUG
	Serial.println("Attempting RC5 decode");
	#endif
	if (decodeRC5(results)) {
		return DECODED;
	}
	#ifdef DEBUG
	Serial.println("Attempting RC6 decode");
	#endif
	if (decodeRC6(results)) {
		return DECODED;
	}
	#ifdef DEBUG
	Serial.println("Attempting Panasonic decode");
	#endif
	if (decodePanasonic(results)) {
		return DECODED;
	}
	#ifdef DEBUG
	Serial.println("Attempting LG decode");
	#endif
	if (decodeLG(results)) {
		return DECODED;
	}
	#ifdef DEBUG
	Serial.println("Attempting JVC decode");
	#endif
	if (decodeJVC(results)) {
		return DECODED;
	}
	#ifdef DEBUG
	Serial.println("Attempting SAMSUNG decode");
	#endif
	if (decodeSAMSUNG(results)) {
		return DECODED;
	}
	// decodeHash returns a hash on any input.
	// Thus, it needs to be last in the list.
	// If you add any decodes, add them before this.
	if (decodeHash(results)) {
		return DECODED;
	}
	// Throw away and start over
	resume();
	return ERR;
}