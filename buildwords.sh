#!/bin/bash

#uses -o from grep to extract words
grep -o '<td>.*</td>' | 

#treats uppercase as lowercase
tr "A-Z" 'a-z'|

#changes ` to '
sed "s/\`/\'/g" | 

#remove <u> </u> tag 
sed 's/<u>//g' |

sed 's/<\/u>//g' |

#remove <td> tages
sed 's/<td>//g' |

sed 's/<\/td>//g' |

#remove empty lines
sed '/^$/d' |

#print only even number lines (removes English words)
awk 'NR % 2 == 0' |

#spaces into newlines
tr " " "\n" |

#remove commas since the spaces dealt with the newlines 
sed 's/,//g' |

#remove empty lines again to get rid of any leftovers
sed '/^$/d' |

#remove non-hawaiian words 
#after all that above has been done; ^ is like not !
sed "/[^pk'mnwlhaeiou]/d" |

#sort the words and gets rid of duplicates
sort -u