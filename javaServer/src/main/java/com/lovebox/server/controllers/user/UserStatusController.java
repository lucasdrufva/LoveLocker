package com.lovebox.server.controllers.user;

import com.lovebox.server.ImageService;
import com.lovebox.server.controllers.requests.StatusTextRequest;
import com.lovebox.server.controllers.responses.StatusResponse;
import com.lovebox.server.models.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;
import org.springframework.web.server.ResponseStatusException;

import java.awt.image.BufferedImage;
import java.io.IOException;
import java.util.Date;
import java.util.Optional;

@RestController
public class UserStatusController {
    @Autowired
    TextRepository textRepository;

    @Autowired
    StatusRepository statusRepository;

    @Autowired
    DeviceRepository deviceRepository;

    @Autowired
    ImageService imageService;

    @Autowired
    ImageRepository imageRepository;

    @PostMapping("/user/device/{deviceName}/status/text")
    public StatusResponse postTextStatus(@RequestBody StatusTextRequest statusText, @PathVariable String deviceName){
        Text dbText = new Text(statusText.getText());
        Status status = new Status();
        status.setDate(new Date());
        status.setNotifier(statusText.getNotifier());
        status.setType(Status.TYPE_TEXT);
        status.setText(textRepository.save(dbText));

        Optional<Device> maybeDevice = deviceRepository.findFirstByDeviceClientName(deviceName);
        if(!maybeDevice.isPresent()){
            throw new ResponseStatusException(HttpStatus.NOT_FOUND);
        }
        Device device = maybeDevice.get();
        status.setDevice(device);

        return StatusResponse.fromStatus(statusRepository.save(status));
    }

    @PostMapping("/user/device/{deviceName}/status/image")
    public StatusResponse postImageStatus(@RequestParam("file") MultipartFile file, @PathVariable String deviceName)  throws Exception{
        if (file.isEmpty()) {
            return null;
        }
        Image dbImage = new Image();
        dbImage.setName(file.getName());
        dbImage.setDate(new Date());

        BufferedImage image = null;
        try {
            image = imageService.createImageFromBytes(file.getBytes());
        }catch (IOException e) {
            e.printStackTrace();
        }

        if(image == null){
            return null;
        }

        dbImage.setParts(imageService.partsFromImage(image, dbImage));

        imageRepository.save(dbImage);
        Status status = new Status();
        status.setDate(new Date());
        status.setNotifier(0);
        status.setType(Status.TYPE_IMAGE);
        status.setImage(dbImage);

        Optional<Device> maybeDevice = deviceRepository.findFirstByDeviceClientName(deviceName);
        if(!maybeDevice.isPresent()){
            throw new ResponseStatusException(HttpStatus.NOT_FOUND);
        }
        Device device = maybeDevice.get();
        status.setDevice(device);

        return StatusResponse.fromStatus(statusRepository.save(status));
    }

}