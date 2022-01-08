import React from 'react';
import { useState, useEffect, useRef, useCallback } from 'react';
import { View, Text, TextInput, Button, FlatList } from 'react-native';
import axios from 'axios';
import {statusTypes} from '../screens/NewPostScreen';
//import {BottomSheetModalProvider} from '@gorhom/bottom-sheet';

import { baseUrl, useAuth } from '../AuthProvider';
import StatusCard from '../components/StatusCard';
import BottomBar from '../components/DeviceBottomBar';
import { useDevice } from '../DeviceContext';

export default function DeviceScreen({ navigation }) {
  const device = useDevice().device;
  const code = device.code;
  const name = device.name;
  const [inputText, onChangeInputText] = useState('');
  const [image, setImage] = useState('');
  const [statuses, setStatuses] = useState([]);

  const auth = useAuth().auth;

  useEffect(() => {
    getStatuses();
  }, []);

  useEffect(() => {
    navigation.addListener('focus', () => {
      getStatuses();
    });
  }, [navigation]);

  function getStatuses() {
    axios
      .get(baseUrl + '/user/device/' + code + '/status', {
        auth: auth,
      })
      .then(response => {
        console.log(response.data);
        setStatuses(response.data);
      })
      .catch(error => {
        console.log(error);
      });
  }

  function postTextStatus(textStatus) {
    axios
      .post(baseUrl + '/user/device/' + code + '/status/text', textStatus, {
        auth: auth,
      })
      .then(response => {
        console.log(response.data);
        onChangeInputText('');
        getStatuses();
      })
      .catch(error => {
        console.log(error);
      });
  }

  return (
    <View style={{ backgroundColor: 'rgba(255,216,222,255)', flex: 1 }}>
      <FlatList
        data={statuses}
        renderItem={status => <StatusCard status={status.item} />}
        keyExtractor={(item, index) => index.toString()}
        contentContainerStyle={{
          justifyContent: 'center',
          alignItems: 'center',
        }}
      />
      {/* <div>
        {statuses.map((status, i) => {
          <StatusCard key={i} status={status.item}/>
        })}
      </div> */}
      <button onClick={() => {
        navigation.navigate('PostStatus', {
          type: statusTypes.TEXT,
        });
      }}>+</button>
      <BottomBar />
    </View>
  );
}
