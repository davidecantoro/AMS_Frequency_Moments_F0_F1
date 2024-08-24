if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  echo "Running simulation for Linux"
  simulazioni/linux_simulazione_esperimento.sh
elif [[ "$OSTYPE" == "darwin"* ]]; then
  echo "Running simulation for Mac OS X"
  if command -v gtime &> /dev/null
  then
      simulazioni/mac_simulazione_esperimento.sh
  else
      echo "Errore: gnu-time non risulta installato, usa brew install gnu-time per installarlo"
  fi
elif [[ "$OSTYPE" == "cygwin" || "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
  echo "Running simulation for Windows"
  simulazioni/linux_simulazione_esperimento.sh  # using wsl for windows
else
  echo "Unknown OS"
fi

